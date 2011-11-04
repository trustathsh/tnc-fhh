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
 
#include "DummyIMV.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <log4cxx/logger.h>
using namespace log4cxx;

static LoggerPtr logger(Logger::getLogger(
		"IMUnit.AbstractIMUnit.AbstractIMV.DummyIMV"));

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
DummyIMV::DummyIMV(TNC_ConnectionID conID, DummyIMVLibrary *pDummyImvLibrary) :
	tncfhh::iml::AbstractIMV(conID, pDummyImvLibrary), msgCountMax(0),
			msgCountCur(0) {
	//
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
DummyIMV::~DummyIMV() {
	//
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result DummyIMV::notifyConnectionChange() {
	LOG4CXX_TRACE(logger, "notifyConnectionChange()");
	// check that TNC handshake has started
	if (this->getConnectionState() == TNC_CONNECTION_STATE_HANDSHAKE) {
		std::string line;
		// reset counter for current messages
		this->msgCountCur = 0;
		// open policy file
		std::ifstream policy(IMV_CONFIG, std::ios::in);
		if (!policy) {
			LOG4CXX_DEBUG(logger, "could not open policy file: " << IMV_CONFIG << ". Using 0 as default value for msgCountMax.");
			this->msgCountMax = 0;
		} else {
			// read first line
			getline(policy, line);
			std::stringstream ss(line);

			// save max number of messages to be sent
			ss >> this->msgCountMax;
			if (ss.fail()) {
				LOG4CXX_DEBUG(logger, "could not parse msg count from policy. Using 0 as default value for msgCountMax.")
				this->msgCountMax = 0;
			}
			// close policy file
			policy.close();
		}
	}
	return TNC_RESULT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result DummyIMV::receiveMessage(TNC_BufferReference message,
		TNC_UInt32 messageLength, TNC_MessageType messageType) {

	// parse message from IMC
	// should look like
	// "DummyIMC message n, action = <access | isolate | none>"
	std::stringstream receive;
	receive.write((const char*) message, messageLength);
	std::string receiveString = receive.str();
	LOG4CXX_TRACE(logger, "Receive Message: " << receiveString)
	size_t pos = receiveString.find_first_of("=");
	std::string action = receiveString.substr(pos + 2, std::string::npos);
	if (!action.compare("allow")) {
		this->actionRecommendation = TNC_IMV_ACTION_RECOMMENDATION_ALLOW;
		this->evaluationResult = TNC_IMV_EVALUATION_RESULT_COMPLIANT;
	}
	if (!action.compare("isolate")) {
		this->actionRecommendation = TNC_IMV_ACTION_RECOMMENDATION_ISOLATE;
		this->evaluationResult = TNC_IMV_EVALUATION_RESULT_NONCOMPLIANT_MINOR;
	}
	if (!action.compare("none")) {
		this->actionRecommendation = TNC_IMV_ACTION_RECOMMENDATION_NO_ACCESS;
		this->evaluationResult = TNC_IMV_EVALUATION_RESULT_NONCOMPLIANT_MAJOR;
	}
	// can we send another message
	if (this->msgCountCur < this->msgCountMax) {
		// send another message to IMV
		this->msgCountCur++;
		std::stringstream send;
		send << "DummyIMV to DummyIMC message " << this->msgCountCur;
		std::string sendString = send.str();
		LOG4CXX_DEBUG(logger, "Send Message: " << sendString)
		this->tncs.sendMessage((unsigned char *) sendString.c_str(),
				sendString.size(), TNC_MESSAGETYPE_FHH_DUMMY);
	}
	// check if necessary amount of messages has been sent
	if (this->msgCountCur == this->msgCountMax) {
		LOG4CXX_DEBUG(logger, "All DummyIMV messages sent. Providing Recommendation: " << this->actionRecommendation << " " << this->evaluationResult)
		this->validationFinished = true;
		this->tncs.provideRecommendation(this->actionRecommendation,
				this->evaluationResult);
	}
	return TNC_RESULT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result DummyIMV::batchEnding() {
	LOG4CXX_TRACE(logger, "batchEnding()");
	return TNC_RESULT_SUCCESS;
}
