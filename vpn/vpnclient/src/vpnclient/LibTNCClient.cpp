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
 
#include "LibTNCClient.h"
#include <iostream>

#include <log4cxx/logger.h>

// FIXME: we only support _one_ active TNCClient
static ifttls::Envelope *uglyStaticEnvelopePointer = NULL;
static bool uglyStaticReceiveIndicator = false;

namespace tnc {

#define LOGNAME "[LibTNC Wrapper]"
static log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger(LOGNAME);

LibTNCClient::LibTNCClient() : wantWrite_(true), wantRead_(false), finished_(false)
{
	LOG4CXX_TRACE(logger, "Constructor");
}

LibTNCClient::~LibTNCClient()
{
	LOG4CXX_TRACE(logger, "Delete connection");
	libtnc_tncc_DeleteConnection(connection);
	libtnc_tncc_Terminate();
	connection = NULL;
	LOG4CXX_TRACE(logger, "Deleted");
}

void LibTNCClient::beginSession()
{
	LOG4CXX_TRACE(logger, "beginSession()");
	/* TODO: configureable? */
	libtnc_tncc_Initialize("/etc/tnc_config");
	libtnc_tncc_PreferredLanguage("en");
	connection = libtnc_tncc_CreateConnection(NULL);
	libtnc_tncc_BeginSession(connection);
	LOG4CXX_TRACE(logger, "beginSession() finished");
}

ifttls::Envelope *LibTNCClient::getMessage()
{
	LOG4CXX_TRACE(logger, "getMessage()");
	if (wantWrite_ && uglyStaticReceiveIndicator) {
		ifttls::Envelope *tmp = uglyStaticEnvelopePointer;
		uglyStaticEnvelopePointer = NULL;
		uglyStaticReceiveIndicator = false;
		wantWrite_ = false;
		wantRead_ = true;
		return tmp;
	} else {
		LOG4CXX_ERROR(logger, "getMessage() bad call!");
		throw 20;
	}
}

void LibTNCClient::putData(ifttls::Envelope *env)
{
	LOG4CXX_TRACE(logger, "putData() value length=" << env->getMessageValueLength());
	if (wantRead_) {
		char * payload = env->getMessageValue();
		uint32_t len = env->getMessageValueLength();
		TNC_Result res = libtnc_tncc_ReceiveBatch(connection, payload, len);
		delete[] payload;
		if (res == TNC_RESULT_SUCCESS) {
			wantWrite_ = true;
			wantRead_ = false;

			/* no message to be sent, so we are finished */
			if (!uglyStaticReceiveIndicator) {
				LOG4CXX_TRACE(logger, "Finished!");
				finished_ = true;
			} else {
				LOG4CXX_TRACE(logger, "Have something to send!!!");
			}
		} else {
			LOG4CXX_ERROR(logger, "putData() failed!");
		}
	} else {
		LOG4CXX_ERROR(logger, "putData() bad call!");
		throw 30;
	}
}


} // namespace tnc

/*
 * This is the callback function needed by libtnc, currently it assumes only
 * one libtnc_tncc_connection is open and uses a static pointer
 * to an envelope to store the message to be sent
 */
TNC_Result TNC_TNCC_SendBatch(libtnc_tncc_connection *conn, const char *buffer,
			      size_t len)
{
	LOG4CXX_TRACE(tnc::logger, "TNC_TNCC_SendBatch()");
	uglyStaticEnvelopePointer = new ifttls::Envelope(ifttls::Envelope::IFT_TNCCS_20_BATCH,
			buffer, len);
	uglyStaticReceiveIndicator = true;

	return TNC_RESULT_SUCCESS;
}
/*
 * Callback normally needed for a TNCS implementation. Linking under
 * Ubuntu leads to errors as of a failing TNC_TNCS_SendBatch function.
 * Therefore, include a skeleton here.
 */
TNC_Result TNC_TNCS_SendBatch( libtnc_tncc_connection *conn, const char *buffer,
			       size_t len)
{
	return TNC_RESULT_SUCCESS;
}

