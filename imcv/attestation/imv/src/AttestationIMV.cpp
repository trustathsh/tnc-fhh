/* 
 * Copyright (C) 2006-2011 Fachhochschule Hannover
 * (University of Applied Sciences and Arts, Hannover)
 * Faculty IV, Dept. of Computer Science
 * Ricklinger Stadtweg 118, 30459 Hannover, Germany
 * 
 * Email: trust@f4-i.fh-hannover.de
 * Website: http://trust.inform.fh-hannover.de/
 * 
 * This file is part of tnc@fhh, an open source 
 * Trusted Network Connect implementation by the Trust@FHH
 * research group at the Fachhochschule Hannover.
 * 
 * tnc@fhh is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * tnc@fhh is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with tnc@fhh; if not, see <http://www.gnu.org/licenses/>
 */
 
#include "AttestationIMV.h"

/* log4cxx includes */
#include <log4cxx/logger.h>
using namespace log4cxx;

#include <cstring>			/* memcpy(), memcmp()	*/
#include <openssl/pem.h>		/* PEM_read_bio_X509()	*/
#include <openssl/x509.h>		/* X509 object handling	*/

#include <iostream>

#include <arpa/inet.h>			/* htons() ntohs()	*/

#include "AttestationIMVLibrary.h"

/* our logger */
static LoggerPtr
logger(Logger::getLogger("IMUnit.AbstractIMUnit.AbstractIMV.AttestationIMV"));


AttestationIMV::AttestationIMV(TNC_ConnectionID conID,
		AttestationIMVLibrary *imclib,
		AttestationIMVPolicyManager *pm )
	: AbstractIMV(conID, imclib)
	, x509Cert(NULL)
	, pKey(NULL)
	, rsa(NULL)
	, policyManager(pm)
	, nothingWrong(true)
	, firstMessage(true)
{
	// empty on purpose
}



AttestationIMV::~AttestationIMV()
{
	cleanup();
}

void AttestationIMV::cleanup()
{
	if (x509Cert != NULL) {
		X509_free(x509Cert);
		x509Cert = NULL;
	}

	if (pKey != NULL) {
		EVP_PKEY_free(pKey);
		pKey = NULL;
	}

	if (rsa != NULL) {
		RSA_free(rsa);
		rsa = NULL;
	}

	/* 
	 * Do not delete the policyManager,
	 * this is done by the IMVLibrary.
	 * Furthermore we might even access
	 * it again.
	 */
}



TNC_Result AttestationIMV::receiveMessage(TNC_BufferReference message,
			TNC_UInt32 length, TNC_MessageType type)
{
	LOG4CXX_TRACE(logger, "receiveMessage()");

	if (!nothingWrong) {
		LOG4CXX_ERROR(logger, "Previous failure... not sending anything...");

	} else if (firstMessage) {
		LOG4CXX_DEBUG(logger, "Received first message, should be the x509 cert");
		firstMessage = !firstMessage;
		if (processFirstMessage(message, length) < 0) {
			return TNC_RESULT_FATAL;
		}

		if (!checkClientKnown()) {
			LOG4CXX_INFO(logger, "Client Certificate unknown. :-(");
			tncs.provideRecommendation(TNC_IMV_ACTION_RECOMMENDATION_NO_ACCESS,
							TNC_IMV_EVALUATION_RESULT_DONT_KNOW);

			// inform imc about decision ?
			// tncs.sendMessage(message, sizeof(message), TNC_MESSAGETYPE_FHH_ATTESTATION);
			//tncs.provideRecommendation(); TNC_IMV_ACTION_RECOMMENDATION_NO_ACCESS
			return TNC_RESULT_SUCCESS;
		}

		entries = policyManager->getPcrEntries();
		quoteType = policyManager->getQuoteType();

		if (quoteType == complete) {
			LOG4CXX_INFO(logger, "Beginning complete mode quoting...");
			sendQuoteRequest(entries);
		} else if (quoteType == single) {
			LOG4CXX_INFO(logger, "Beginning single mode quoting...");
			currentEntry = 0;
			vector<PcrEntry> vec(1, entries[currentEntry]);
			sendQuoteRequest(vec);
		}

	} else {
		LOG4CXX_TRACE(logger, "Received second message, should be a signature");


		if (quoteType == complete) {
			LOG4CXX_INFO(logger, "Complete Quoting... Checking signature...");
			if (1 != RSA_verify(NID_sha1, quoteDigest, SHA_DIGEST_LENGTH, message, length, rsa)) {
				LOG4CXX_FATAL(logger, "Bad signature :-(");
				tncs.provideRecommendation(TNC_IMV_ACTION_RECOMMENDATION_NO_ACCESS,
								TNC_IMV_EVALUATION_RESULT_COMPLIANT);
			} else {
				LOG4CXX_INFO(logger, "Good signature :-)");
				tncs.provideRecommendation(TNC_IMV_ACTION_RECOMMENDATION_ALLOW,
								TNC_IMV_EVALUATION_RESULT_COMPLIANT);
			}
		} else {
			LOG4CXX_DEBUG(logger, "Single Quoting...");
			LOG4CXX_DEBUG(logger, "Checking signatur for pcr" << entries[currentEntry].index);
			if (1 != RSA_verify(NID_sha1, quoteDigest, SHA_DIGEST_LENGTH, message, length, rsa)) {
				LOG4CXX_FATAL(logger, "Bad signature for pcr" << entries[currentEntry].index << " :-(");
				nothingWrong = false;
				tncs.provideRecommendation(TNC_IMV_ACTION_RECOMMENDATION_NO_ACCESS,
								TNC_IMV_EVALUATION_RESULT_COMPLIANT);
				return TNC_RESULT_SUCCESS;
			}
			LOG4CXX_INFO(logger, "Good signature for pcr" << entries[currentEntry].index << " :-)");

			if (++currentEntry < entries.size()) {
				vector<PcrEntry> vec(1, entries[currentEntry]);
				sendQuoteRequest(vec);
			} else {
				// finished
				LOG4CXX_INFO(logger, "Everything was fine :-)");
				tncs.provideRecommendation(TNC_IMV_ACTION_RECOMMENDATION_ALLOW,
								TNC_IMV_EVALUATION_RESULT_COMPLIANT);
			}
		}


	}
	return TNC_RESULT_SUCCESS;
}

/*
 * FIXME: Needs some splitting
 */
int AttestationIMV::sendQuoteRequest(std::vector<PcrEntry>& entries)
{
	unsigned int maxPcr = 0;
	int maskLength = -1;
	std::vector<PcrEntry>::iterator it = entries.begin();
	std::vector<PcrEntry>::iterator end = entries.end();

	for (/*see above*/; it != end; it++) {
		LOG4CXX_TRACE(logger, "Checking PCR " << (*it).index);
		if ((*it).index > maxPcr)
			maxPcr = (*it).index;
	}
	LOG4CXX_DEBUG(logger, "Highest PCR is " << maxPcr);

	maskLength = maxPcr / 8 + 1;

	// maskLength needs to be at least two bytes long
	if (maskLength < 2) {
		maskLength = 2;
	}

	// 2 bytes length field + maskLength bytes bitmask + 20 byte nonce
	unsigned char *buf = new unsigned char[2 + maskLength + 20];		
	unsigned char *pcrs = buf + 2;

	memset(buf, 0, 2 + maskLength);
	*(UINT16*)buf = htons(maskLength);


	it = entries.begin();

	// set the corresponding bits in the bitmask
	// documentation about the bitmask can be found
	// in the specification
	// FIXME: Where and which version?
	//
	for (/*see above*/; it != end; it++) {
		unsigned int i = (*it).index;
		pcrs[i/8] |= (1 << (i % 8));
	}

	LOG4CXX_TRACE(logger, "Generating nonce...");
	if (RAND_bytes(pcrs + maskLength, 20) == 0) {
			LOG4CXX_FATAL(logger, "RAND_bytes() failed!!!");
			nothingWrong = false;
			delete[] buf;
			return -1;
	}

	calculateQuoteDigest(maskLength, buf, entries);

	LOG4CXX_INFO(logger, "Bitmask:");
	for (int i = 0; i < maskLength + 20; i++) {
		printf("%02hhx", pcrs[i]);
	}
	printf("\n");

	LOG4CXX_INFO(logger, "Sending PcrSelection to IMC...");
	tncs.sendMessage(buf, 2 + maskLength + 20, TNC_MESSAGETYPE_FHH_ATTESTATION);
	delete[] buf;

	return 0;
}


/*
 * This method builds up the digest we expect to be sent by the IMC
 *
 *  the following steps are done:
 *
 *   build a TCPA_PCR_COMPOSITE structure which contains (pcrCompositeBuf)
 *     TCPA_PCR_SELECTION structure (bitmask length network order + length bytes bitmask)
 *     UINT32 (network order) gives the number of bytes following (pcr entries * 20)
 *     TCPA_PCRVALUE[] with the pcr values
 *
 *   SHA1(pcrCompositeBuf)
 *
 *   build a TCPA_QUOTE_INFO structure which contains
 *     4 bytes of version
 *     4 bytes 'Q' 'U' 'O' 'T'
 *     20 byte SHA1 of TCPA_PCR_COMPOSITE
 *     20 byte nonce
 *
 *   SHA1(TCPA_QUOTE_INFO) gives quoteDigest
 *
 * FIXME: Needs some splitting
 */
int AttestationIMV::calculateQuoteDigest(unsigned int maskLen, unsigned char *buf,
						std::vector<PcrEntry>& entries)
{
	LOG4CXX_TRACE(logger, "calculateQuoteDigest()");
	TNC_Buffer		qinfo[48];

	unsigned int		compositeLen = 2 + maskLen + 4 + entries.size() * 20;
	LOG4CXX_TRACE(logger, "allocating " << compositeLen << " bytes for composite...");
	TNC_BufferReference	pcrCompositeBuf = new TNC_Buffer[compositeLen];
	TNC_BufferReference	pcrCompositePcrPos = pcrCompositeBuf + 2 + maskLen + 4;

	//PCR Selection
	memcpy(pcrCompositeBuf, buf, 2 + maskLen);
	//Length of PCR Values
	*(UINT32 *)(pcrCompositeBuf + 2 + maskLen) = htonl(entries.size() * 20);

	std::vector<PcrEntry>::iterator it = entries.begin();
	std::vector<PcrEntry>::iterator end = entries.end();

	//Copy PCR Values in Buffer
	for (/* see above */; it != end; it++) {
		memcpy(pcrCompositePcrPos, (*it).value, 20);
		pcrCompositePcrPos += 20;
	}


	// Version number
	qinfo[0] = 1; qinfo[1] = 1; qinfo[2] = 0; qinfo[3] = 0;
	// Magic QUOT value
	qinfo[4] = 'Q'; qinfo[5] = 'U'; qinfo[6] = 'O'; qinfo[7] = 'T';

	// Create SHA1 hash over pcrComposite structure and store it after
	// version and magic QUOT value into qinfo
	SHA1(pcrCompositeBuf, compositeLen, qinfo + 8);

	// Copy the nonce behind the pcrComposite SHA1 hash
	memcpy(qinfo + 28, buf + 2 + maskLen, 20);

	// Create a SHA1 hash over the quote structure and store it into
	// quoteDigest. That is the digest we expect the IMC to send in
	// the next round.
	SHA1(qinfo, sizeof(qinfo), quoteDigest);

	delete[] pcrCompositeBuf;

	return 0;
}

bool AttestationIMV::checkClientKnown()
{
	LOG4CXX_TRACE(logger, "checkClientKnown()");
	char				hex[4];		/* to store a hex digit	  */
	std::string			strfp;		/* string for fingerprint */
	unsigned int			x509digestLen;
	unsigned char			md[EVP_MAX_MD_SIZE];
	const EVP_MD			*sha1Digest = EVP_sha1();
	X509_digest(x509Cert, sha1Digest, md, &x509digestLen);

	LOG4CXX_DEBUG(logger, "Size of fingerprint:" << x509digestLen);

	for (unsigned int j = 0; j < x509digestLen; j++) {
		snprintf(hex, 4, "%02X%s", md[j],
				(j != (x509digestLen - 1)) ? ":" : "");
		strfp.append(hex);
	}

	LOG4CXX_DEBUG(logger, "The fingerprint: " << strfp);

	return policyManager->isAikKnown(strfp);
}



int AttestationIMV::processFirstMessage(TNC_BufferReference message, TNC_UInt32 length)
{
	LOG4CXX_TRACE(logger, "processFirstMessage()");
	if (loadX509Cert(message, length) < 0) {
		LOG4CXX_FATAL(logger, "Could not create X509"
					" certificate object");
		nothingWrong = false;
		return -1;						/* return */
	}

	if (loadPKey() < 0) {
		LOG4CXX_FATAL(logger, "Could not create EVP_PKEY object");
		nothingWrong = false;
			return -1;					/* return */
		}

	if (loadRSA() < 0) {
		LOG4CXX_FATAL(logger, "Could not create RSA object");
		nothingWrong = false;
		return -1;						/* return */
	}
	return 0;
}



bool AttestationIMV::isASN1(TNC_BufferReference message, TNC_UInt32 length)
{
	unsigned char tag, n, *p;
	TNC_UInt32 len = 0;

	if (length < 2)
	{
		return false;
	}
	length -= 2;

	/* start ASN.1 parsing at the head of the message */
	p = (unsigned char *)message;
	
	/* check if tag is an ASN1_SEQUENCE */
	tag = *p++;
	if (tag != (V_ASN1_CONSTRUCTED | V_ASN1_SEQUENCE))
	{
		return false;
	}

	/* is there a single length byte? */
	n = *p++;
	if (!(n & 0x80))
	{
		return (n == length);	
	}
	n &= 0x7f;
	
	if (n > length || n > 4)
	{
		return false;
	}
	length -= n;

	/* compute length from n length bytes */
	while (n--)
	{
		len = (len << 8) + *p++; 
	}
	return (len == length);
}



int AttestationIMV::loadX509Cert(TNC_BufferReference message, TNC_UInt32 length)
{
	LOG4CXX_TRACE(logger, "loadX509Cert()");

	BIO *bio = BIO_new_mem_buf(message, length);

	if (bio == NULL) {
		LOG4CXX_FATAL(logger, "Could not create BIO object");
		return -1;
	}
	if (isASN1(message, length)) {
		x509Cert = d2i_X509_bio(bio, NULL);
	} else {
		x509Cert = PEM_read_bio_X509(bio, NULL, NULL, NULL);
	}
	if (x509Cert == NULL) {
		LOG4CXX_FATAL(logger, "Could not create X509 object");
		BIO_free(bio);
		return -1;
	}
	LOG4CXX_INFO(logger, "X509 certificate successfully received");
	BIO_free(bio);
	return 0;
}



int AttestationIMV::loadPKey(void)
{
	LOG4CXX_TRACE(logger, "loadPKey()");
	pKey = X509_get_pubkey(x509Cert);

	if (pKey == NULL)
		return -1;
	return 0;
}



int AttestationIMV::loadRSA(void)
{
	LOG4CXX_TRACE(logger, "loadRSA()");
	rsa = EVP_PKEY_get1_RSA(pKey);

	if (rsa == NULL)
		return -1;
	return 0;
}



TNC_Result AttestationIMV::notifyConnectionChange(TNC_ConnectionState newState)
{
	LOG4CXX_TRACE(logger, "notifyConnectionChange()");
	if (newState == TNC_CONNECTION_STATE_HANDSHAKE) {
		LOG4CXX_INFO(logger, "Start new TNC Handshake");
		nothingWrong = true;
		firstMessage = true;

		cleanup();

	}

	return TNC_RESULT_SUCCESS;
}



TNC_Result AttestationIMV::solicitRecommendation()
{
	/* something failed with the cert */
	if (!nothingWrong) {
	tncs.provideRecommendation(TNC_IMV_ACTION_RECOMMENDATION_NO_ACCESS,
					TNC_IMV_EVALUATION_RESULT_DONT_KNOW);
	} else {
		tncs.provideRecommendation(TNC_IMV_ACTION_RECOMMENDATION_NO_RECOMMENDATION,
					TNC_IMV_EVALUATION_RESULT_DONT_KNOW);
	}

	return TNC_RESULT_SUCCESS;
}
