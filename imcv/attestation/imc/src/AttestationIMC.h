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
 * AttestationIMC
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>01.03.10 - create class skeleton (awe)</li>
 *   <li>24.03.10 - renamed to AttestationIMC (awe)</li>
 * </ul>
 *
 * @date 24.03.10
 * @author Arne Welzel (awe)
 *
 */

#ifndef ATTESTATIONIMC_H_
#define ATTESTATIONIMC_H_


/* some defines */
#define ATTESTATIONIMC_X509_CERT		"x509_certificate_file"
#define	ATTESTATIONIMC_AIK_KEY			"aik_key_file"


/* Trousers TSS */
#include <trousers/tss.h>
#include <trousers/trousers.h>

/* IFIMC */
#include <tcg/tnc/tncifimc.h>

#include "imunit/imc/AbstractIMC.h"
#include "AttestationIMCLibrary.h"
#include "PcrSelection.h"


using namespace tncfhh::iml;
using namespace std;

class AttestationIMC : public AbstractIMC
{


private:
	
	/* context handle */
	TSS_HCONTEXT 	hContext;
	/* tpm handle */
	TSS_HTPM	hTPM;

	/* srk handle */
	TSS_HKEY	hSRK;

	/* srk policy handle */
	TSS_HPOLICY srkPolicy;

	/* aik handle */
	TSS_HKEY	hAIK;

	/* number of PCRs */
	UINT32		nPCR;


	/* able to start a handshake */
	bool				initialized;
	bool				certificateSent;

	/* place to store the x509 certificate */
	TNC_UInt32			certificateLength;
	TNC_BufferReference		certificate;

	/* place to store the aik blob */
	TNC_UInt32			aikBlobLength;
	TNC_BufferReference		aikBlob;

	TNC_UInt32			signatureLength;
	TNC_BufferReference		signature;

	string				certificateFile;
	string				aikBlobFile;


public:
	AttestationIMC(TNC_ConnectionID conID, AttestationIMCLibrary *imclib);

	~AttestationIMC();

	TNC_Result beginHandshake();

	TNC_Result receiveMessage(TNC_BufferReference message,
	                          TNC_UInt32 length, TNC_MessageType messageType);

	TNC_Result batchEnding();

	TNC_Result notifyConnectionChange();

private:
	/**
	 * Parse config file and set variables concerning:
	 *
	 * 	- path to public key file
	 *  - path to the aik blob file
	 */
	int loadConfigFile(void);


	/*
	 * Process one config line, can be comment...
	 */
	int processConfigLine(string configLine);
	void processCertificateLine(string configLine);
	void processAikKeyLine(string configLine);




	int loadX509Certificate(void);

	int loadAikBlob(void);

	/* helper to read files */
	int loadBlobToBuf(TNC_BufferReference *buf, const char *filename);

	 /*
	  * connect to tss,
	  * get tpm object, get number of pcrs
	  */
	int initTpmStuff(void);

	/*
	 * set number of Pcrs in nPCR using the before initialized context
	 */
	int setNumberOfPcrs(void);

	/*
	 * do the quote
	 */
	int doQuote(PcrSelection& ps, TNC_BufferReference nonce);

	void cleanup1(void);
	void cleanup2(void);
	void cleanup3(void);
	void cleanup4(void);
	void cleanup5(void);
};

#endif // ATTESTATIONIMC_H_
