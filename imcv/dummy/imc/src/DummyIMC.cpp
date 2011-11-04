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
 
#include "DummyIMC.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <log4cxx/logger.h>

using namespace log4cxx;

static LoggerPtr logger(Logger::getLogger("IMUnit.AbstractIMUnit.AbstractIMC.DummyIMC"));

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
DummyIMC::DummyIMC(TNC_ConnectionID conID, DummyIMCLibrary *pDummyIMCLibrary)
	:AbstractIMC(conID, pDummyIMCLibrary), localFileAction(""), count(0)
{
	//
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
DummyIMC::~DummyIMC()
{
	//
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result DummyIMC::beginHandshake()
{
	LOG4CXX_TRACE(logger, "beginHandshake()");
	// reset counter
	this->count = 0;
	// read local file
	std::ifstream dummyIMCfile(IMC_CONFIG);
	if(!dummyIMCfile)
	{
		LOG4CXX_DEBUG(logger, "could not open dummy_imc_file: " << IMC_CONFIG << ". Using isolate as recommendation." );
		this->localFileAction = "isolate";
	} else {
		getline(dummyIMCfile, this->localFileAction);
		if( this->localFileAction.compare("allow") != 0 &&
			this->localFileAction.compare("isolate") != 0 &&
			this->localFileAction.compare("none") != 0)
		{
			LOG4CXX_DEBUG(logger, "recommendation from " << IMC_CONFIG << " not valid. Using isolate as recommendation.");
			this->localFileAction = "isolate";
		}
		dummyIMCfile.close();
	}
	std::stringstream ss;
	ss << "DummyIMC message " << this->count << ", ";
	ss << "action = " << this->localFileAction;
	std::string sendString = ss.str();
	LOG4CXX_TRACE(logger, "Send Message: " << sendString);
	this->tncc.sendMessage((unsigned char*)sendString.c_str(), sendString.size(), TNC_MESSAGETYPE_FHH_DUMMY);
	return TNC_RESULT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result DummyIMC::receiveMessage(TNC_BufferReference message,
		                          TNC_UInt32 messageLength,
		                          TNC_MessageType messageType)
{
	LOG4CXX_TRACE(logger, "Received Message: " << message);
	this->count++;
	std::stringstream ss;
	ss << "DummyIMC message " << this->count << ", ";
	ss << "action = " << this->localFileAction;

	std::string sendString = ss.str();

	LOG4CXX_TRACE(logger, "Send Message: " << sendString);
	this->tncc.sendMessage((unsigned char*)sendString.c_str(), sendString.size(), TNC_MESSAGETYPE_FHH_DUMMY);

	return TNC_RESULT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result DummyIMC::batchEnding()
{
	LOG4CXX_TRACE(logger, "batchEnding");
	return TNC_RESULT_SUCCESS;
}
