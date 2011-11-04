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
 
#include "TNCC.h"
#include "IMCLibrary.h"

#include <log4cxx/logger.h>

using namespace log4cxx;
using namespace log4cxx::helpers;

namespace tncfhh {

namespace iml {

static LoggerPtr logger(Logger::getLogger("IMUnit.TNCC"));

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNCC::TNCC(TNC_ConnectionID cid, IMCLibrary *pImcLibrary):
	cid(cid), pImcLibrary(pImcLibrary)
{
	// intentionally left blank
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNCC::~TNCC()
{
	// intentionally left blank
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TNCC::sendMessage(TNC_BufferReference message, TNC_UInt32 messageLength, TNC_MessageType messageType) throw (ResultException)
{
//	Possible binary data in message... Just comment this out and leave it
//	to the IMV whether output is done or not
//	LOG4CXX_TRACE(logger, "sendMessage message: " << message << " length: "<< messageLength  << "type: " << messageType);
	LOG4CXX_TRACE(logger, "sendMessage message: length: "<< messageLength  << "type: " << messageType);
	if (pImcLibrary->tnccFunctionPointers.sendMessagePointer == NULL)
	{
		LOG4CXX_WARN(logger, "No function pointer for sending message to TNCC. Throwing ResultException ...")
		throw ResultException("Could not send a message. No sendMessage function pointer", TNC_RESULT_FATAL);
	}

	TNC_Result result = pImcLibrary->tnccFunctionPointers.sendMessagePointer(pImcLibrary->getIMUnitLibraryID(), this->cid, message, messageLength, messageType);

	if (result != TNC_RESULT_SUCCESS)
	{
		LOG4CXX_WARN(logger, "Error when calling sendMessage for TNCC. Throwing ResultException ...")
		throw ResultException(std::string("Exception on TNCC::sendMessage."), result);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TNCC::sendMessage(TNC_BufferReference message, TNC_UInt32 messageLength, TNC_VendorID vendorId, TNC_MessageSubtype messageSubtype) throw (ResultException)
{
	this->sendMessage(message, messageLength, (vendorId << 8) | (messageSubtype & 0xff));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TNCC::requestHandshakeRetry(TNC_RetryReason reason) throw (ResultException)
{
	LOG4CXX_TRACE(logger, "requestHandshakeRetry reason: " << reason);
	if (pImcLibrary->tnccFunctionPointers.requestHandshakeRetryPointer == NULL)
	{
		LOG4CXX_WARN(logger, "No function pointer for requestHandshakeRetry in TNCC. Throwing ResultException ...")
		throw ResultException("Could not requestHandshakeRetry. No requestHandshakeRetry function pointer", TNC_RESULT_FATAL);
	}

	TNC_Result result = pImcLibrary->tnccFunctionPointers.requestHandshakeRetryPointer(pImcLibrary->getIMUnitLibraryID(), this->cid, reason);

	if (result != TNC_RESULT_SUCCESS)
	{
		LOG4CXX_WARN(logger, "Error when calling requestHandshakeRetry for TNCC. Throwing ResultException ...")
		throw ResultException(std::string("Exception on requestHandshakeRetry."), result);
	}
}

}  // namespace iml

}  // namespace tncfhh
