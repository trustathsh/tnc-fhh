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
 
#include "PlatidIMV.h"

/* log4cxx includes */
#include <log4cxx/logger.h>
using namespace log4cxx;

#include <cstring>			/* memcpy(), memcmp() */
#include <openssl/pem.h>		/* PEM_read_bio_X509() */
#include <openssl/x509.h>

#include <iostream>

/* our logger */
static LoggerPtr
logger(Logger::getLogger("IMUnit.AbstractIMUnit.AbstractIMV.PlatidIMV"));


PlatidIMV::PlatidIMV(TNC_ConnectionID conID,
		PlatidIMVLibrary *imclib,
		CertManager *cm )
	: AbstractIMV(conID, imclib)
	, x509Cert(NULL)
	, pKey(NULL)
	, rsa(NULL)
	, randNonce(NULL)
	, randNonceLength(0)
	, nonceSent(false)
	, certManager(cm)
	, nothingWrong(true)
{  }



PlatidIMV::~PlatidIMV()
{
	cleanup();
}

void PlatidIMV::cleanup()
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

	if (randNonce != NULL) {
		delete[] randNonce;
		randNonce = NULL;
	}

	/*					*
	 * 	Don't delete the certManager,	*
	 * 	it's done by the IVMLibrary	*
	 *					*/
}


TNC_Result PlatidIMV::receiveMessage(TNC_BufferReference message,
			TNC_UInt32 length, TNC_MessageType type)
{
	LOG4CXX_TRACE(logger, "receiveMessage()");
	TNC_IMV_Action_Recommendation verifyResult;

	if (!nothingWrong) {
		LOG4CXX_ERROR(logger, "Previous error.. not sending anything...");

	} else if (!nonceSent) {
		LOG4CXX_DEBUG(logger, "Received first message, should be"
							" the X509 Certificate");
		processFirstMessage(message, length);

		if (nothingWrong) {
			generateAndSendNonce();
		}
	} else {
		LOG4CXX_INFO(logger, "Received signature of nonce");
		verifyResult = verifySignature(message, length);

		tncs.provideRecommendation(verifyResult,
					TNC_IMV_EVALUATION_RESULT_DONT_KNOW);
	}

	return TNC_RESULT_SUCCESS;
}



int PlatidIMV::processFirstMessage(TNC_BufferReference message, TNC_UInt32 length)
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



int PlatidIMV::loadX509Cert(TNC_BufferReference message, TNC_UInt32 length)
{
	LOG4CXX_TRACE(logger, "loadX509Cert()");

	BIO *bio = BIO_new_mem_buf(message, length);

	if (bio == NULL) {
		LOG4CXX_FATAL(logger, "Could not create BIO object");
		return -1;
	}
	x509Cert = PEM_read_bio_X509(bio, NULL, NULL, NULL);

	if (x509Cert == NULL) {
		LOG4CXX_FATAL(logger, "Could not create X509 object");
		BIO_free(bio);
		return -1;
	}
	LOG4CXX_INFO(logger, "X509 certificate successfully received");
	BIO_free(bio);
	return 0;
}



int PlatidIMV::loadPKey(void)
{
	LOG4CXX_TRACE(logger, "loadPKey()");
	pKey = X509_get_pubkey(x509Cert);

	if (pKey == NULL)
		return -1;
	return 0;
}



int PlatidIMV::loadRSA(void)
{
	LOG4CXX_TRACE(logger, "loadRSA()");
	rsa = EVP_PKEY_get1_RSA(pKey);

	if (rsa == NULL)
		return -1;
	return 0;
}



int PlatidIMV::generateAndSendNonce()
{
	LOG4CXX_INFO(logger, "generateAndSendNonce()");

	randNonceLength = RSA_size(rsa) - 11;
	randNonce = new unsigned char[randNonceLength];

	if (randNonce == NULL) {
		LOG4CXX_FATAL(logger, "Could not allocate memory for randNonce");
		nothingWrong = false;
		return -1;
	}

	if (RAND_bytes(randNonce, randNonceLength) == 0) {
		LOG4CXX_FATAL(logger, "RAND_bytes() failed!!!");
		nothingWrong = false;
		return -1;
	} else {
		tncs.sendMessage(randNonce, randNonceLength, TNC_MESSAGETYPE_FHH_PLATID);
		nonceSent = !nonceSent;
	}

	return 0;
}



TNC_IMV_Action_Recommendation
PlatidIMV::verifySignature(TNC_BufferReference sig, TNC_UInt32 length)
{
	LOG4CXX_TRACE(logger, "verifySignatur()");

	TNC_IMV_Action_Recommendation	rec		=
					TNC_IMV_ACTION_RECOMMENDATION_NO_ACCESS;
	TNC_BufferReference		decryptedSig	= NULL;
	TNC_UInt32			dsiglen		= 0;

	char				issuername[512];/* to store the issuer	  */
	char				hex[4];		/* to store a hex digit	  */
	std::string 			striss;		/* string for issuer	  */
	std::string			strfp;		/* string for fingerprint */

	unsigned int			x509digestLen;
	unsigned char			md[EVP_MAX_MD_SIZE];
	const EVP_MD			*sha1Digest = EVP_sha1();

	if (length == 0) {
		LOG4CXX_ERROR(logger, "Message length zero! No Access!");
		return rec;				/* RETURN */
	}


	X509_NAME_oneline(X509_get_issuer_name(x509Cert), issuername, 256);

	LOG4CXX_DEBUG(logger, "Certificate issuername = " << issuername);

	X509_digest(x509Cert, sha1Digest, md, &x509digestLen);

	LOG4CXX_DEBUG(logger, "Size of fingerprint:" << x509digestLen);

	for (unsigned int j = 0; j < x509digestLen; j++) {
		snprintf(hex, 4, "%02X%s", md[j],
				(j != (x509digestLen - 1)) ? ":" : "");
		strfp.append(hex);
	}

	LOG4CXX_DEBUG(logger, "The fingerprint: " << strfp);

	striss = issuername;

	if (certManager->isCertKnown(issuername, strfp)) {
		LOG4CXX_INFO(logger, "Certificate is known, --> GOOD!");
	} else {
		return rec;				/* RETURN */
	}


	LOG4CXX_TRACE(logger, "Request memory for decrypted signature");
	decryptedSig = new TNC_Buffer[RSA_size(rsa)];

	if (decryptedSig == NULL) {
		LOG4CXX_ERROR(logger, "Could not allocate memory"
				" to store the decrypted signature");
		return TNC_IMV_ACTION_RECOMMENDATION_NO_RECOMMENDATION;
	}

	LOG4CXX_DEBUG(logger, "Length of message: " << length);

	dsiglen = RSA_public_decrypt(length, sig, decryptedSig, rsa, RSA_PKCS1_PADDING);

	LOG4CXX_INFO(logger, "Length of decrypted signature: " << dsiglen);

	if (!memcmp(randNonce, (const char *)decryptedSig, randNonceLength)) {
		LOG4CXX_INFO(logger, "Signature is correct --> VERY GOOD!");
		rec = TNC_IMV_ACTION_RECOMMENDATION_ALLOW;
	} else {
		LOG4CXX_INFO(logger, "Bad signature received!")
		rec = TNC_IMV_ACTION_RECOMMENDATION_NO_ACCESS;
	}

	delete[] decryptedSig;

	return rec;
}



TNC_Result PlatidIMV::notifyConnectionChange(TNC_ConnectionState newState)
{
	LOG4CXX_TRACE(logger, "notifyConnectionChange()");

	if (newState == TNC_CONNECTION_STATE_HANDSHAKE && nonceSent) {
		nothingWrong = true;
		nonceSent = false;

		cleanup();
	}

	return TNC_RESULT_SUCCESS;
}



TNC_Result PlatidIMV::solicitRecommendation()
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
