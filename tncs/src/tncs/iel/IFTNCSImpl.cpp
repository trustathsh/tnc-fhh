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
 
#include "Coordinator.h"
#include "../exception/ResultException.h"
#include <log4cxx/logger.h>
#include <string.h>

/*
 * TODO_Comment
 * 
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>25.02.2008 - create file (mbs)</li>
 *   <li>21.07.2009 - redesign 0.6.0 (ib)</li>
 * </ul>
 *
 * @date 25.02.2008
 * @author Mike Steinmetz (mbs)
 * @author Ingo Bente (ib)
 */

/* for logging */
static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("IF-IMV.TNCS"));


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result TNC_TNCS_ReportMessageTypes(TNC_IMVID imvID, TNC_MessageTypeList supportedTypes, TNC_UInt32 typeCount)
{
	LOG4CXX_DEBUG(logger, "TNC_TNCS_ReportMessageTypes");
	try {
		// check parameters
		if ( (supportedTypes == NULL && typeCount != 0) ||
			 (supportedTypes != NULL && typeCount == 0)){
			LOG4CXX_WARN(logger, "Invalid Parameter.");
			return TNC_RESULT_INVALID_PARAMETER;
		}
		tncfhh::iel::coordinator.reportMessageTypes(imvID, supportedTypes, typeCount);
		return TNC_RESULT_SUCCESS;
	} catch (tncfhh::iel::ResultException &e) {
		LOG4CXX_WARN(logger, e.what());
		return e.getReturnValue();
	} catch (std::exception &e) {
		LOG4CXX_WARN(logger, e.what());
		return TNC_RESULT_OTHER;
	} catch (...) {
		LOG4CXX_WARN(logger, "UNKOWN ERROR");
		return TNC_RESULT_FATAL;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result TNC_TNCS_SendMessage( TNC_IMVID imvID, TNC_ConnectionID connectionID, TNC_BufferReference message, TNC_UInt32 messageLength, TNC_MessageType messageType)
{
	LOG4CXX_DEBUG(logger, "TNC_TNCS_SendMessage");
	
	try {
		// check messageType
		if( (message == NULL && messageLength != 0) ||
			((messageType & 0xFF) == 0xFF) || // subtype any
			((messageType >> 8) == 0xFFFFFF)) // vendorID any
		{
			LOG4CXX_WARN(logger, "Invalid Parameter.");
			return TNC_RESULT_INVALID_PARAMETER;
		}
		tncfhh::iel::coordinator.sendMessage(imvID, connectionID, message, messageLength, messageType);
		return TNC_RESULT_SUCCESS;
	} catch (tncfhh::iel::ResultException &e) {
		LOG4CXX_WARN(logger, e.what());
		return e.getReturnValue();
	} catch (std::exception &e) {
		LOG4CXX_WARN(logger, e.what());
		return TNC_RESULT_OTHER;
	} catch (...) {
		LOG4CXX_WARN(logger, "UNKOWN ERROR");
		return TNC_RESULT_FATAL;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result TNC_TNCS_RequestHandshakeRetry(TNC_IMVID imvID, TNC_ConnectionID connectionID, TNC_RetryReason reason)
{
	LOG4CXX_DEBUG(logger, "TNC_TNCS_RequestHandshakeRetry");

	try {
		// check retry reason is in range
		if(reason < TNC_RETRY_REASON_IMV_IMPORTANT_POLICY_CHANGE ||
		   reason > TNC_RETRY_REASON_IMV_PERIODIC)
		{
			LOG4CXX_WARN(logger, "Invalid Parameter.");
			return TNC_RESULT_INVALID_PARAMETER;
		}
		tncfhh::iel::coordinator.requestHandshakeRetry(imvID, connectionID, reason);
		return TNC_RESULT_SUCCESS;
	} catch (tncfhh::iel::ResultException &e) {
		LOG4CXX_WARN(logger, e.what());
		return e.getReturnValue();
	} catch (std::exception &e) {
		LOG4CXX_WARN(logger, e.what());
		return TNC_RESULT_OTHER;
	} catch (...) {
		LOG4CXX_WARN(logger, "UNKOWN ERROR");
		return TNC_RESULT_FATAL;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result TNC_TNCS_ProvideRecommendation(TNC_IMVID imvID, TNC_ConnectionID connectionID, TNC_IMV_Action_Recommendation recommendation, TNC_IMV_Evaluation_Result evaluation)
{
	LOG4CXX_DEBUG(logger, "TNC_TNCS_ProvideRecommendation");

	try {
		// check parameters
		if(recommendation > TNC_IMV_ACTION_RECOMMENDATION_NO_RECOMMENDATION ||
		   evaluation > TNC_IMV_EVALUATION_RESULT_DONT_KNOW)
		{
			LOG4CXX_WARN(logger, "Invalid Parameter.");
			return TNC_RESULT_INVALID_PARAMETER;
		}
		tncfhh::iel::coordinator.provideRecommendation(imvID, connectionID, recommendation, evaluation);
		return TNC_RESULT_SUCCESS;
	} catch (tncfhh::iel::ResultException &e) {
		LOG4CXX_WARN(logger, e.what());
		return e.getReturnValue();
	} catch (std::exception &e) {
		LOG4CXX_WARN(logger, e.what());
		return TNC_RESULT_OTHER;
	} catch (...) {
		LOG4CXX_WARN(logger, "UNKOWN ERROR");
		return TNC_RESULT_FATAL;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result TNC_TNCS_GetAttribute(TNC_IMVID imvID, TNC_ConnectionID connectionID, TNC_AttributeID attributeID, TNC_UInt32 bufferLength, /*out*/ TNC_BufferReference buffer, /*out*/ TNC_UInt32 *pOutValueLength)
{
	LOG4CXX_DEBUG(logger, "TNC_TNCS_GetAttribute");

	try {
		// check parameters
		if(connectionID == TNC_CONNECTIONID_ANY)
		{
			LOG4CXX_WARN(logger, "Invalid Parameter. connectionID == TNC_CONNECTIONID_ANY");
			return TNC_RESULT_INVALID_PARAMETER;
		}
		tncfhh::iel::coordinator.getAttribute(imvID, connectionID, attributeID, bufferLength, buffer, pOutValueLength);
		return TNC_RESULT_SUCCESS;
	} catch (tncfhh::iel::ResultException &e) {
		LOG4CXX_WARN(logger, e.what());
		return e.getReturnValue();
	} catch (std::exception &e) {
		LOG4CXX_WARN(logger, e.what());
		return TNC_RESULT_OTHER;
	} catch (...) {
		LOG4CXX_WARN(logger, "UNKOWN ERROR");
		return TNC_RESULT_FATAL;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result TNC_TNCS_SetAttribute(TNC_IMVID imvID, TNC_ConnectionID connectionID, TNC_AttributeID attributeID, TNC_UInt32 bufferLength, TNC_BufferReference buffer)
{
	LOG4CXX_DEBUG(logger, "TNC_TNCS_SetAttribute");

	try {
		// check parameters
		if(connectionID == TNC_CONNECTIONID_ANY)
		{
			LOG4CXX_WARN(logger, "Invalid Parameter. connectionID == TNC_CONNECTIONID_ANY");
			return TNC_RESULT_INVALID_PARAMETER;
		}
		tncfhh::iel::coordinator.setAttribute(imvID, connectionID, attributeID, bufferLength, buffer);
		return TNC_RESULT_SUCCESS;
	} catch (tncfhh::iel::ResultException &e) {
		LOG4CXX_WARN(logger, e.what());
		return e.getReturnValue();
	} catch (std::exception &e) {
		LOG4CXX_WARN(logger, e.what());
		return TNC_RESULT_OTHER;
	} catch (...) {
		LOG4CXX_WARN(logger, "UNKOWN ERROR");
		return TNC_RESULT_FATAL;
	}
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result TNC_TNCS_BindFunction(TNC_IMVID imvID, char *functionName, /*out*/ void **pOutfunctionPointer)
{
	LOG4CXX_DEBUG(logger, "TNC_TNCS_BindFunction");

	if(functionName == NULL ||
	   pOutfunctionPointer == NULL)
	{
		LOG4CXX_WARN(logger, "Invalid Parameter.");
        return TNC_RESULT_INVALID_PARAMETER;
	}
	
    if (!strcmp(functionName, "TNC_TNCS_ReportMessageTypes")) {
        *pOutfunctionPointer = (void *) TNC_TNCS_ReportMessageTypes;
    } else if (!strcmp(functionName, "TNC_TNCS_SendMessage")) {
        *pOutfunctionPointer = (void *) TNC_TNCS_SendMessage;
    } else if(!strcmp(functionName, "TNC_TNCS_RequestHandshakeRetry")){
        *pOutfunctionPointer = (void *) TNC_TNCS_RequestHandshakeRetry;
    } else if(!strcmp(functionName, "TNC_TNCS_ProvideRecommendation")){
        *pOutfunctionPointer = (void *) TNC_TNCS_ProvideRecommendation;
    } else if(!strcmp(functionName, "TNC_TNCS_GetAttribute")){
    	// TODO implement getAttribute
//        *pOutfunctionPointer = NULL;
        *pOutfunctionPointer = (void *) TNC_TNCS_GetAttribute;
    } else if(!strcmp(functionName, "TNC_TNCS_SetAttribute")){
    	// TODO implement setAttribute
//        *pOutfunctionPointer = NULL;
        *pOutfunctionPointer = (void *) TNC_TNCS_SetAttribute;
    } else {
    	LOG4CXX_WARN(logger, "Unknown function " << functionName << " requested!");
        *pOutfunctionPointer = NULL;
        return TNC_RESULT_INVALID_PARAMETER;
    }

    LOG4CXX_TRACE(logger, "Return function pointer " << functionName);

    return TNC_RESULT_SUCCESS;
}
