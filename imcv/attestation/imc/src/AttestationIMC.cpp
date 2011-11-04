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
 
#include "AttestationIMC.h"

/* log4cxx includes */
#include <log4cxx/logger.h>
using namespace log4cxx;

/* openssl includes */
#include <openssl/rsa.h>
#include <openssl/engine.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>

/* used for file reading */
#include <fstream>
#include <iostream>
#include <algorithm>

#include <arpa/inet.h>


#include "attestationimcConfig.h"


static LoggerPtr
logger(Logger::getLogger("IMUnit.AbstractIMUnit.AbstractIMC.AttestationIMC"));


AttestationIMC::AttestationIMC(TNC_ConnectionID conID, AttestationIMCLibrary *imclib)
	: AbstractIMC(conID, imclib)
	, initialized(false)
	, certificateSent(false)
	, certificateLength(-1)
	, certificate(NULL)
	, aikBlobLength(-1)
	, aikBlob(NULL)
{
	LOG4CXX_INFO(logger, "Constructor");
}



AttestationIMC::~AttestationIMC()
{
	LOG4CXX_TRACE(logger, "Destructor");
	if (certificate != NULL) {
		delete[] certificate;
	}

	if (aikBlob != NULL) {
		delete[] aikBlob;
	}
	cleanup5();
}



int AttestationIMC::loadConfigFile(void)
{
	LOG4CXX_TRACE(logger, "loadConfigFile()");
	ifstream cfgfile(IMC_CONFIG);
	string line;

	if (cfgfile.is_open()) {
		while (!cfgfile.eof()) {
			getline(cfgfile, line);
			processConfigLine(line);
		}
	} else {
		LOG4CXX_FATAL(logger, "Could not open " << IMC_CONFIG);
		return -1;
	}

	LOG4CXX_TRACE(logger, "Certificate File = " << certificateFile);
	LOG4CXX_TRACE(logger, "AikBlob File     = " << aikBlobFile);

	if (certificateFile.length() == 0 || aikBlobFile.length() == 0) {
		LOG4CXX_FATAL(logger, "Failed parsing config file!");
		return -1;
	}
	return 0;
}



int AttestationIMC::processConfigLine(string configLine)
{
	LOG4CXX_TRACE(logger, "processConfigLine()");

	if (!(configLine.length() > 0)) {
		LOG4CXX_DEBUG(logger, "Found empty line");

	} else if (configLine.at(0) == '#') {
		LOG4CXX_DEBUG(logger, "Found comment line");

	} else if (!configLine.compare(0, strlen(ATTESTATIONIMC_X509_CERT),
			ATTESTATIONIMC_X509_CERT)) {
		LOG4CXX_DEBUG(logger, "process certificate entry");
		processCertificateLine(configLine);

	} else if (!configLine.compare(0, strlen(ATTESTATIONIMC_AIK_KEY),
			ATTESTATIONIMC_AIK_KEY)) {
		LOG4CXX_DEBUG(logger, "process aik key entry");
		processAikKeyLine(configLine);

	} else {
		LOG4CXX_WARN(logger, "Found unknown entry in config");
	}

	return 0;
}



void AttestationIMC::processCertificateLine(string line)
{
	unsigned int i;
	if ((i = line.find_first_of(' ')) != std::string::npos) {
		certificateFile = line.substr(i+1, line.length() - (i + 1));
		LOG4CXX_INFO(logger, "Certificate File = " << certificateFile);
	}
}

void AttestationIMC::processAikKeyLine(string line)
{
	unsigned int i;
	if ((i = line.find_first_of(' ')) != std::string::npos) {
		aikBlobFile = line.substr(i+1, line.length() - (i + 1));
		LOG4CXX_INFO(logger, "AikBlob File = " << aikBlobFile);
	}
}

int AttestationIMC::loadX509Certificate(void)
{
	if (certificate != NULL) {
		delete[] certificate;
	}
	certificateLength = loadBlobToBuf(&certificate, certificateFile.c_str());
	if (certificateLength < 0) {
		LOG4CXX_FATAL(logger, "Failed to load certifciate!");
	}
	return certificateLength;
}



int AttestationIMC::loadAikBlob()
{
	if (aikBlob != NULL) {
		delete[] aikBlob;
	}
	aikBlobLength = loadBlobToBuf(&aikBlob, aikBlobFile.c_str());
	if (aikBlobLength < 0) {
		LOG4CXX_FATAL(logger, "Failed to load AIK blob!");
	}
	return aikBlobLength;
}


/*
 * Read a file into buffer... Allocating as much memory as needed.
 */
int AttestationIMC::loadBlobToBuf(TNC_BufferReference *buf, const char *filename)
{
	int len;
	LOG4CXX_TRACE(logger, "open file: " << filename);
	ifstream file(filename, ios::binary);

	if (file.is_open()) {
		file.seekg(0, ios::end);
		len = file.tellg();
		file.seekg(0, ios::beg);
		LOG4CXX_TRACE(logger, "Requesting " << len << " bytes buffer");
		*buf = new TNC_Buffer[len];
		file.read((char *)(*buf), len);
		file.close();
	} else {
		LOG4CXX_FATAL(logger, "Could not open file " << filename);
		len = -1;
	}
	return len;
}



/**
 * load config and cert
 * send our x509 cert
 */
TNC_Result AttestationIMC::beginHandshake()
{

	if (loadConfigFile() < 0) {
		LOG4CXX_FATAL(logger, "BAD loadConfigFile()");
		return TNC_RESULT_FATAL;
	}

	if (loadX509Certificate() < 0) {
		LOG4CXX_FATAL(logger, "BAD loadX509Certificate()");
		return TNC_RESULT_FATAL;
	}

	if (loadAikBlob() < 0) {
		LOG4CXX_FATAL(logger, "BAD loadAikBlob()");
		return TNC_RESULT_FATAL;
	}

	if (initTpmStuff() < 0) {
		LOG4CXX_FATAL(logger, "Could not init TPM stuff");
		cleanup5();
		return TNC_RESULT_FATAL;
	}

	if (setNumberOfPcrs() < 0) {
		LOG4CXX_FATAL(logger, "Could not set Number of PCRs");
		return TNC_RESULT_FATAL;
	}

	initialized = true;

	tncc.sendMessage(certificate, certificateLength, TNC_MESSAGETYPE_FHH_ATTESTATION);
	certificateSent = true;
	return TNC_RESULT_SUCCESS;
}


/**
 * Message will consist of:
 * 2 bytes length for bitmasklength
 * bitmasklength bitmask
 * 20 bytes sha1 nonce
 *
 * minimum length therefore ist 23
 */
#define ATTESTATION_MIN_RECV_LENGTH 23
TNC_Result AttestationIMC::receiveMessage(TNC_BufferReference message,
	                          TNC_UInt32 length, TNC_MessageType messageType)
{
	UINT16 bitMaskLength;
	if (certificateSent) {
		if (length < ATTESTATION_MIN_RECV_LENGTH) {
			LOG4CXX_FATAL(logger, "Message length=" << length);
			LOG4CXX_FATAL(logger, "Expected more...");
			return TNC_RESULT_INVALID_PARAMETER;
		}

		bitMaskLength = ntohs(*(UINT16*)message);

		if ((2 + bitMaskLength + 20) != length) {
			LOG4CXX_FATAL(logger, "Message length is wrong!!!");
			return TNC_RESULT_INVALID_PARAMETER;
		}

		PcrSelection ps(message+2, bitMaskLength);

		if (ps.getHighestPcr() >= nPCR) {
			LOG4CXX_FATAL(logger, "IMV requested PCR" << ps.getHighestPcr() <<
					" but we have only " << ps.getPcrCount());
			return TNC_RESULT_INVALID_PARAMETER;
		}

		doQuote(ps, message + 2 + bitMaskLength);

	} else {
		LOG4CXX_FATAL(logger, "Didn't sent the certificate!");
		return TNC_RESULT_FATAL;
	}
	return TNC_RESULT_SUCCESS;
}

/* macro which returns -1 if a is not TSS_RESULT
 * c represents the cleanup function to be called
 * before returning
 */
#define CHECK_TSS_RETURN(a, b, c)						\
	do { 									\
		if (a != TSS_SUCCESS) {						\
			if (b != NULL)						\
				LOG4CXX_FATAL(logger, b);			\
			LOG4CXX_FATAL(logger, Trspi_Error_String(a));		\
			c();							\
			return -1;						\
		}								\
	} while (0);


static void noop() {
	return;
}

void AttestationIMC::cleanup1(void) {
	Tspi_Context_Close(hContext);
}

void AttestationIMC::cleanup2(void) {
	Tspi_Context_CloseObject(hContext, hSRK);
	cleanup1();
}

void AttestationIMC::cleanup3(void) {
	Tspi_Context_CloseObject(hContext, srkPolicy);
	cleanup2();
}

void AttestationIMC::cleanup4(void) {
	Tspi_Context_CloseObject(hContext, hAIK);
	cleanup3();
}

void AttestationIMC::cleanup5(void) {
	Tspi_Context_CloseObject(hContext, hTPM);
	cleanup4();
}


int AttestationIMC::initTpmStuff() {

	TSS_UUID srkUuid = TSS_UUID_SRK;
	BYTE wks[] = TSS_WELL_KNOWN_SECRET;
	TSS_RESULT res;
	res = Tspi_Context_Create(&hContext);
	CHECK_TSS_RETURN(res, "Context_Create", noop);
	res = Tspi_Context_Connect(hContext, NULL);
	CHECK_TSS_RETURN(res, "Context_Connect", cleanup1);
	res = Tspi_Context_LoadKeyByUUID(hContext, TSS_PS_TYPE_SYSTEM,
			srkUuid, &hSRK);
	CHECK_TSS_RETURN(res, "Load_SRK", cleanup1);
	res = Tspi_GetPolicyObject(hSRK, TSS_POLICY_USAGE, &srkPolicy);
	CHECK_TSS_RETURN(res, "Get_SRK_Policy", cleanup2);
	res = Tspi_Policy_SetSecret(srkPolicy, TSS_SECRET_MODE_SHA1, 20, wks);
	CHECK_TSS_RETURN(res, "Set_SRK_Secret", cleanup2);
	res = Tspi_Context_LoadKeyByBlob(hContext, hSRK, aikBlobLength, aikBlob, &hAIK);
	CHECK_TSS_RETURN(res, "Load aik blob", cleanup3);
	res = Tspi_Context_GetTpmObject(hContext, &hTPM);
	CHECK_TSS_RETURN(res, "Get_TPM_Object", cleanup4);

	return 0;
}

int AttestationIMC::setNumberOfPcrs(void)
{
	TSS_RESULT res;

	UINT32 tpmProp = TSS_TPMCAP_PROP_PCR;
	UINT32 tmpLen;
	BYTE *tmpBuf;

	res = Tspi_TPM_GetCapability(hTPM, TSS_TPMCAP_PROPERTY, sizeof(tpmProp),
			(BYTE*)&tpmProp, &tmpLen, &tmpBuf);
	CHECK_TSS_RETURN(res, "Get_Capability", cleanup5);
	nPCR = *(UINT32*)tmpBuf;
	LOG4CXX_INFO(logger, "The TPM has " << nPCR << " PCRs");
	Tspi_Context_FreeMemory(hContext, tmpBuf);
	return 0;
}

int AttestationIMC::doQuote(PcrSelection& ps, TNC_BufferReference nonce)
{
	LOG4CXX_TRACE(logger, "doQuote()");
	LOG4CXX_TRACE(logger, "ps.length=" << ps.getPcrCount());
	LOG4CXX_TRACE(logger, "ps.max=" << ps.getHighestPcr());
	TSS_HPCRS hPCRs;
	TSS_RESULT res;
	TSS_VALIDATION valid;
	res = Tspi_Context_CreateObject(hContext, TSS_OBJECT_TYPE_PCRS,
			TSS_PCRS_STRUCT_INFO, &hPCRs);
	CHECK_TSS_RETURN(res, "Create Pcr struct", cleanup5);

	std::list<long>::iterator it = ps.getPcrList().begin();
	std::list<long>::iterator itend = ps.getPcrList().end();
	LOG4CXX_TRACE(logger, "setting all wanted PCRs");

	for (/* look above */;it != itend; it++) {
		long cur = *it;
		LOG4CXX_TRACE(logger, "Current index = " << cur);
		res = Tspi_PcrComposite_SelectPcrIndex(hPCRs, *it);
		CHECK_TSS_RETURN(res, "SelectPcrIndex", cleanup5);
	}
	LOG4CXX_TRACE(logger, "after setting index");

	/* magic */
	valid.ulExternalDataLength = 20;
	valid.rgbExternalData = nonce;

	res = Tspi_TPM_Quote(hTPM, hAIK, hPCRs, &valid);
	CHECK_TSS_RETURN(res, "Quote Failure", cleanup5);
	LOG4CXX_TRACE(logger, "after Tspi_TPM_Quote()");

	LOG4CXX_INFO(logger, "Quote Bytes IMC:");
	fflush(stdout);
	for (unsigned int i = 0; i < valid.ulDataLength; i++) {
		printf("%0X%s", valid.rgbData[i],
				(i != valid.ulDataLength -1) ? ":" : "\n");
	}

	LOG4CXX_TRACE(logger, "ulDataLength " << valid.ulDataLength);

	tncc.sendMessage(valid.rgbValidationData, valid.ulValidationDataLength,
			TNC_MESSAGETYPE_FHH_ATTESTATION);

	Tspi_Context_FreeMemory(hContext, valid.rgbValidationData);
}

TNC_Result AttestationIMC::batchEnding()
{
	return TNC_RESULT_SUCCESS;
}



TNC_Result AttestationIMC::notifyConnectionChange()
{
	return TNC_RESULT_SUCCESS;
}

