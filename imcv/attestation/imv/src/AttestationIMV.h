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
 
/**
 * AttestationIMV
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>08.03.2010 - create class skeleton (ib)</li>
 *   <li>24.03.2010 - renamed to AttestationIMV (awe)</li>
 * </ul>
 *
 * @date 08.03.10
 * @author Ingo Bente (ib)
 *
 */

#ifndef ATTESTATIONIMV_H_
#define ATTESTATIONIMV_H_

/* default includes */
#include "tcg/tnc/tncifimc.h"
#include "imunit/imv/AbstractIMV.h"
#include "AttestationIMVLibrary.h"
#include "AttestationIMVPolicyManager.h"

/* openssl includes */
#include <openssl/rsa.h>
#include <openssl/rand.h>
#include <openssl/sha.h>

#include <vector>

using namespace tncfhh::iml;
using namespace std;


class AttestationIMV : public tncfhh::iml::AbstractIMV
{
private:
	X509				*x509Cert;
	EVP_PKEY			*pKey;
	RSA					*rsa;
	AttestationIMVPolicyManager	*policyManager;
	bool				nothingWrong;
	bool				firstMessage;
	TNC_Buffer			quoteDigest[SHA_DIGEST_LENGTH];
	QuoteType			quoteType;
	vector<PcrEntry>	entries;
	unsigned int		currentEntry;

public:
	AttestationIMV(TNC_ConnectionID conID,
			AttestationIMVLibrary *imclib,
			AttestationIMVPolicyManager * pm);

	~AttestationIMV();

	TNC_Result receiveMessage(TNC_BufferReference message, TNC_UInt32 length,
			TNC_MessageType type);

	TNC_Result notifyConnectionChange(TNC_ConnectionState newState);

	TNC_Result solicitRecommendation();

private:
	/**
	 * Uses the next three methods to process the first message of the
	 * IMC which contains the X509 Certificate. We need to extract
	 * the Public Key and the corresponding RSA object.
	 */
	int processFirstMessage(TNC_BufferReference message, TNC_UInt32 length);

	/**
	 * Detect if a blob is ASN.1 encoded
	 */
	bool isASN1(TNC_BufferReference message, TNC_UInt32 length);

	/**
	 * The first message should be the X509 Certificate,
	 * this method creates a X509 object to work with.
	 * OpenSSLs BIO is used to do this.
	 *
	 * returns 0 on success -1 on failure
	 */
	int loadX509Cert(TNC_BufferReference message, TNC_UInt32 length);


	/**
	 * Extract a EVP_PKEY object from the X509 Certificate
	 *
	 * returns 0 on success -1 on failure
	 */
	int loadPKey(void);

	/**
	 * Extract a RSA object from the EVP_PKEY object
	 *
	 * return 0 on success -1 on failure
	 */
	int loadRSA(void);


	/**
	 * Generate a nonce with RAND_bytes() from Openssl
	 */
	
	bool checkClientKnown();

	int sendQuoteRequest(std::vector<PcrEntry>& entries);

	int calculateQuoteDigest(unsigned int maskLen,
			unsigned char *buf, std::vector<PcrEntry>& entries);

	void cleanup();

};

#endif // ATTESTATIONIMV_H_
