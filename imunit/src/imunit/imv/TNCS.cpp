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
 
#include "TNCS.h"
#include "IMVLibrary.h"

#include <log4cxx/logger.h>
using namespace log4cxx;

namespace tncfhh {

namespace iml {

static LoggerPtr logger(Logger::getLogger("IMUnit.TNCS"));

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNCS::TNCS(TNC_ConnectionID cid, IMVLibrary *pImvLibrary):
	cid(cid), pImvLibrary(pImvLibrary)
{
	// intentionally left blank
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *                               TNCS::~TNCS()                               *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

TNCS::~TNCS()
{
	// intentionally left blank
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TNCS::sendMessage(TNC_BufferReference message, TNC_UInt32 messageLength, TNC_MessageType messageType) throw (ResultException)
{
//	Possible binary data in message... Just comment this out and leave it
//	to the IMV whether output is done or not
//	LOG4CXX_TRACE(logger, "sendMessage message: " << message << " length: "<< messageLength  << "type: " << messageType);
	LOG4CXX_TRACE(logger, "sendMessage message: length: "<< messageLength  << "type: " << messageType);
	if (pImvLibrary->tncsFunctionPointers.sendMessagePointer == NULL)
	{
		LOG4CXX_WARN(logger, "No function pointer for sending message to TNCS. Throwing ResultException ...")
		throw ResultException("Could not send a message, because I do not have the functionpointer", TNC_RESULT_FATAL);
	}

	TNC_Result result = pImvLibrary->tncsFunctionPointers.sendMessagePointer(pImvLibrary->getIMUnitLibraryID(), cid, message, messageLength, messageType);
	if (result != TNC_RESULT_SUCCESS)
	{
		LOG4CXX_WARN(logger, "Error when calling sendMessage for TNCS. Throwing ResultException ...")
		throw ResultException(std::string("Exception on send a message."), result);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TNCS::sendMessage(TNC_BufferReference message, TNC_UInt32 messageLength, TNC_VendorID vendorId, TNC_MessageSubtype messageSubtype) throw (ResultException)
{
	this->sendMessage(message, messageLength, (vendorId << 8) | (messageSubtype & 0xff));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TNCS::requestHandshakeRetry(TNC_RetryReason reason) throw (ResultException)
{
	LOG4CXX_TRACE(logger, "requestHandshakeRetry reason: " << reason);
	if (pImvLibrary->tncsFunctionPointers.requestHandshakeRetryPointer == NULL)
	{
		LOG4CXX_WARN(logger, "No function pointer for requestHandshakeRetry in TNCS. Throwing ResultException ...")
		throw ResultException("Could not requestHandshakeRetry, because i haven't the functionpointer", TNC_RESULT_FATAL);
	}

	TNC_Result result = pImvLibrary->tncsFunctionPointers.requestHandshakeRetryPointer(pImvLibrary->getIMUnitLibraryID(), cid, reason);
	if (result != TNC_RESULT_SUCCESS)
	{
		LOG4CXX_WARN(logger, "Error when calling requestHandshakeRetry for TNCS. Throwing ResultException ...")
		throw ResultException(std::string("Exception on requestHandshakeRetry."), result);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void TNCS::provideRecommendation(TNC_IMV_Action_Recommendation recommendation, TNC_IMV_Evaluation_Result evaluation) throw (ResultException)
{
	LOG4CXX_TRACE(logger, "provideRecommendation recommendation: " << recommendation << " evaluation: " << evaluation);
	if (pImvLibrary->tncsFunctionPointers.provideRecommendationPointer == NULL)
	{
		LOG4CXX_WARN(logger, "No function pointer for provideRecommendation in TNCS. Throwing ResultException ...")
		throw ResultException("Could not provide recommendation, because i haven't the functionpointer", TNC_RESULT_FATAL);
	}

	TNC_Result result = pImvLibrary->tncsFunctionPointers.provideRecommendationPointer(pImvLibrary->getIMUnitLibraryID(), cid, recommendation, evaluation);

	if (result != TNC_RESULT_SUCCESS)
	{
		LOG4CXX_WARN(logger, "Error when calling provideRecommendation for TNCS. Throwing ResultException ...")
		throw ResultException(std::string("Exception on provide recommendation."), result);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool TNCS::getAttribute(/*in*/ TNC_AttributeID attributeID, /*in*/ TNC_UInt32 bufferLength, /*out*/ TNC_BufferReference buffer, /*out*/ TNC_UInt32 *pOutValueLength) throw (ResultException)
{
	LOG4CXX_TRACE(logger, "getAttribute attributeID: " << attributeID << " bufferLength: " << bufferLength);
	if (pImvLibrary->tncsFunctionPointers.getAttributePointer == NULL)
	{
		LOG4CXX_INFO(logger, "No function pointer for getAttribute in TNCS. Don't call getAttribute.")
		return false;
	}

	TNC_Result result = pImvLibrary->tncsFunctionPointers.getAttributePointer(pImvLibrary->getIMUnitLibraryID(), cid, attributeID, bufferLength, buffer, pOutValueLength);

	if (result != TNC_RESULT_SUCCESS)
	{
		LOG4CXX_WARN(logger, "Error when calling getAttribute for TNCS. Throwing ResultException ...")
		throw ResultException(std::string("Exception on get attribute."), result);
	}
	return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool TNCS::supportGetAttribute()
{
	return pImvLibrary->tncsFunctionPointers.getAttributePointer != NULL;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool TNCS::setAttribute(TNC_AttributeID attributeID, TNC_UInt32 bufferLength, TNC_BufferReference buffer) throw (ResultException)
{
	LOG4CXX_TRACE(logger, "setAttribute attributeID: " << attributeID << " bufferLength: " << bufferLength);
	if (pImvLibrary->tncsFunctionPointers.setAttributePointer == NULL)
	{
		LOG4CXX_INFO(logger, "No function pointer for setAttribute in TNCS. Don't call setAttribute.")
		return false;
	}
	TNC_Result result = pImvLibrary->tncsFunctionPointers.setAttributePointer(pImvLibrary->getIMUnitLibraryID(), cid, attributeID, bufferLength, buffer);

	if (result != TNC_RESULT_SUCCESS)
	{
		LOG4CXX_WARN(logger, "Error when calling setAttribute in TNCS. Throwing ResultException ...")
		throw ResultException(std::string("Exception on set attribute."), result);
	}
	return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool TNCS::supportSetAttribute()
{
	return pImvLibrary->tncsFunctionPointers.setAttributePointer != NULL;
}

}  // namespace iml

}  // namespace tncfhh
