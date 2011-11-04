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
 
#include "IMVLibrary.h"

#include <sstream>
#include <iomanip>
#include <log4cxx/logger.h>

using namespace log4cxx;

namespace tncfhh {

namespace iml {

static LoggerPtr logger(Logger::getLogger("IMUnit.IMUnitLibrary.IMVLibrary"));

IMVLibrary::IMVLibrary()
{
//	LOG4CXX_TRACE(logger, "IMVLibrary()");
}

IMVLibrary::~IMVLibrary()
{
//	LOG4CXX_TRACE(logger, "~IMVLibrary()");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void * IMVLibrary::getTNCSFunctionPointer(const char *functionName, TNC_BindFunctionPointer tncBindFunction) throw (ResultException)
{
	LOG4CXX_TRACE(logger, this->imUnitLibraryName << "::getTNCSFunctionPointer(" << functionName << ", " << tncBindFunction << ")");
    void *tncFunctionPointer = NULL;
    tncBindFunction(this->imUnitLibraryID, const_cast<char*>(functionName), &tncFunctionPointer);
    if (tncFunctionPointer == NULL) {
        throw ResultException(std::string("TNCS function was not found: ") + functionName, TNC_RESULT_FATAL);
    }
    return tncFunctionPointer;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
tncfhh::iml::AbstractIMV * IMVLibrary::getImvInstances(TNC_ConnectionID id) throw (ResultException)
{
	std::map<TNC_ConnectionID, AbstractIMV *>::iterator result = imvInstances.find(id);
	if (result == imvInstances.end())
	{
		std::stringstream ss;
		ss << "No IMV for connection ID " << id << "!";
		throw ResultException(ss.str(), TNC_RESULT_INVALID_PARAMETER);
	}

	return imvInstances[id];
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void IMVLibrary::checkProvideBindFunction(TNC_IMVID imvID, TNC_TNCS_BindFunctionPointer bindFunction) throw (ResultException)
{
	checkIMUnitLibraryIsInitialized();
	checkIMUnitID(imvID);
	if(bindFunction == NULL){
		throw ResultException("Pointer to bindFunction == NULL.", TNC_RESULT_INVALID_PARAMETER);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result IMVLibrary::provideBindFunction(TNC_IMVID imvID, TNC_TNCS_BindFunctionPointer bindFunction)
{
	LOG4CXX_DEBUG(logger, this->imUnitLibraryName << "::provideBindFunction(" << imvID << ", " << bindFunction << ")");
    try {
    	// check preconditions
    	checkProvideBindFunction(imvID, bindFunction);
		// check parameter
    	if (bindFunction != NULL) {
    		// set the tncsFunctionPointers
    	    this->tncsFunctionPointers.bindFunctionPointer = bindFunction;
    	    this->tncsFunctionPointers.provideRecommendationPointer = (TNC_TNCS_ProvideRecommendationPointer) getTNCSFunctionPointer("TNC_TNCS_ProvideRecommendation", bindFunction);
    	    this->tncsFunctionPointers.reportMessageTypesPointer = (TNC_TNCS_ReportMessageTypesPointer) getTNCSFunctionPointer("TNC_TNCS_ReportMessageTypes", bindFunction);
    	    this->tncsFunctionPointers.requestHandshakeRetryPointer = (TNC_TNCS_RequestHandshakeRetryPointer) getTNCSFunctionPointer("TNC_TNCS_RequestHandshakeRetry", bindFunction);
    	    this->tncsFunctionPointers.sendMessagePointer = (TNC_TNCS_SendMessagePointer) getTNCSFunctionPointer("TNC_TNCS_SendMessage", bindFunction);
    	    // OPTIONAL TNCS functions
			try {
				this->tncsFunctionPointers.getAttributePointer = (TNC_TNCS_GetAttributePointer) getTNCSFunctionPointer("TNC_TNCS_GetAttribute", bindFunction);
    		} catch (ResultException e) {
    			this->tncsFunctionPointers.getAttributePointer = NULL;
    		}
    		try {
    			this->tncsFunctionPointers.setAttributePointer = (TNC_TNCS_SetAttributePointer) getTNCSFunctionPointer("TNC_TNCS_SetAttribute", bindFunction);
    		} catch (ResultException e) {
    			this->tncsFunctionPointers.setAttributePointer = NULL;
    		}

    		// Report messages types to TNC server
    	    this->tncsFunctionPointers.reportMessageTypesPointer(this->imUnitLibraryID, this->messageTypeList, this->typeCount);
    	}
   	    // Everything is OK
   	    return TNC_RESULT_SUCCESS;
	} catch (ResultException& e) {
		// Oh shit, we detect error!
		LOG4CXX_WARN(logger, this->imUnitLibraryName << " caught ResultException: " << e.what() << ", result=" << e.getReturnValue());
	    return e.getReturnValue();
	} catch (std::exception &e) {
		// Oh shit, we detect an std error!
		LOG4CXX_WARN(logger, this->imUnitLibraryName << " caught std::exception: " << e.what());
	    return TNC_RESULT_FATAL;
    } catch (...) {
		// Detect unkwnown error!
		LOG4CXX_WARN(logger, this->imUnitLibraryName << " UNKNOWN ERROR");
	    return TNC_RESULT_FATAL;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void IMVLibrary::checkNotifyConnectionChange(TNC_IMVID imvID, TNC_ConnectionID connectionID, TNC_ConnectionState newState) throw (ResultException)
{
	std::stringstream ss;
	checkIMUnitLibraryIsInitialized();
	checkIMUnitID(imUnitLibraryID);
	if((newState < TNC_CONNECTION_STATE_CREATE) ||
	   (newState > TNC_CONNECTION_STATE_DELETE)){
		ss << "Invalid Connection State: " << newState;
		throw ResultException(ss.str(), TNC_RESULT_INVALID_PARAMETER);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result IMVLibrary::notifyConnectionChange(TNC_IMVID imvID, TNC_ConnectionID connectionID, TNC_ConnectionState newState)
{
	LOG4CXX_DEBUG(logger, this->imUnitLibraryName << "::notifyConnectionChange(" << imvID << ", " << connectionID << ", " << newState << ")");
	AbstractIMV * imv = NULL;
	TNC_Result result = TNC_RESULT_FATAL;

	try {
		// check preconditions
		checkNotifyConnectionChange(imvID, connectionID, newState);
		switch (newState){
			case TNC_CONNECTION_STATE_CREATE:
				{
					/* check that imv does not exist */
					bool exist = false;
					try {
						getImvInstances(connectionID); // throws exception if imv not found
						exist = true;
					} catch (ResultException& e) {
						// expected Exception
						LOG4CXX_TRACE(logger, "expected exception: No IMV for connection ID " << connectionID << " and connectionState = CREATE");
					}
					if (exist) throw ResultException("TNC_IMV_NotifyConnectionChange(TNC_CONNECTION_STATE_CREATE): IMV already exist.", TNC_RESULT_ILLEGAL_OPERATION);

					/* Create concrete IMV instance */
					LOG4CXX_DEBUG(logger, "Library " << this->imUnitLibraryName << " creating IMV instance for conID " << connectionID);
					AbstractIMV * tmp = this->createNewImvInstance(connectionID);
					if (!tmp) throw ResultException("createNewIMVInstance(..) return NULL", TNC_RESULT_FATAL);

					imvInstances[connectionID] = tmp;
					imv = getImvInstances(connectionID);
				}
				break;
				case TNC_CONNECTION_STATE_DELETE:
		            // Connection will be deleted. Remove IMV instance from map.
		            imv = getImvInstances(connectionID);
		            imvInstances.erase(connectionID);
					break;
				case TNC_CONNECTION_STATE_ACCESS_ALLOWED:
				case TNC_CONNECTION_STATE_ACCESS_ISOLATED:
				case TNC_CONNECTION_STATE_ACCESS_NONE:
				case TNC_CONNECTION_STATE_HANDSHAKE:
		            imv = getImvInstances(connectionID);
					break;
				default:
					throw ResultException("Unknown TNC_ConnectionState!", TNC_RESULT_INVALID_PARAMETER);
			}

		    result = imv->notifyConnectionChange(newState);
		} catch (ResultException& e) {
			// Oh shit, we detect error!
			LOG4CXX_WARN(logger, this->imUnitLibraryName << " caught ResultException: " << e.what() << ", result=" << e.getReturnValue());
			result = e.getReturnValue();
		} catch (std::exception &e) {
			// Oh shit, we detect an std error!
			LOG4CXX_WARN(logger, this->imUnitLibraryName << " caught std::exception: " << e.what());
			result = TNC_RESULT_FATAL;
		} catch (...) {
			// Detect unkwnown error!
			LOG4CXX_WARN(logger, this->imUnitLibraryName << " UNKNOWN ERROR");
			result = TNC_RESULT_FATAL;
		}

	    // delete imv if necessary
	    if (newState == TNC_CONNECTION_STATE_DELETE){
			LOG4CXX_WARN(logger, this->imUnitLibraryName << " deleting imv for connection ID " << connectionID << " imv ID " << imvID);
	    	delete imv;
	    	imv = NULL;
	    }

	    // reset round counter if imv exists and connection state handshake
	    if(imv && (newState == TNC_CONNECTION_STATE_HANDSHAKE))
	    	imv->round = 0;

	    return result;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void IMVLibrary::checkSolicitRecommendation(TNC_IMVID imvID, TNC_ConnectionID connectionID) throw (ResultException)
{
	checkIMUnitLibraryIsInitialized();
	checkIMUnitID(imvID);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result IMVLibrary::solicitRecommendation(TNC_IMVID imvID, TNC_ConnectionID connectionID)
{
	LOG4CXX_DEBUG(logger, this->imUnitLibraryName << "::solicitRecommendation(" << imvID << ", " << connectionID << ")");
    try {
    	// check preconditions
    	checkSolicitRecommendation(imvID, connectionID);
	    TNC_UInt32 ret = getImvInstances(connectionID)->solicitRecommendation();
	    return ret;
	} catch (ResultException& e) {
		// Oh shit, we detect error!
		LOG4CXX_WARN(logger, this->imUnitLibraryName << " caught ResultException: " << e.what() << ", result=" << e.getReturnValue());
	    return e.getReturnValue();
	} catch (std::exception &e) {
		// Oh shit, we detect an std error!
		LOG4CXX_WARN(logger, this->imUnitLibraryName << " caught std::exception: " << e.what());
	    return TNC_RESULT_FATAL;
    } catch (...) {
		// Detect unkwnown error!
		LOG4CXX_WARN(logger, this->imUnitLibraryName << " UNKNOWN ERROR");
	    return TNC_RESULT_FATAL;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result IMVLibrary::receiveMessage(TNC_IMVID imvID, TNC_ConnectionID connectionID,
		                              TNC_BufferReference messageBuffer, TNC_UInt32 messageLength,
		                              TNC_MessageType messageType)
{
	LOG4CXX_DEBUG(logger, this->imUnitLibraryName << "::receiveMessage("
			<< "imvID=" << imvID << ", "
			<< "conID=" << connectionID << ", "
			<< "messageBuffer=0x" << std::setw(sizeof(void*)) << std::setfill('0') << std::hex << (void*)messageBuffer << ", "
			<< "messageLength=" << messageLength << ", "
			<< "messageType=0x" << std::setw(8) << std::setfill('0') << std::hex << messageType << ")");

	try {
		checkReceiveMessage(imvID, messageBuffer, messageLength, messageType);
		return getImvInstances(connectionID)->receiveMessage(messageBuffer, messageLength, messageType);
	} catch (ResultException& e) {
		// Oh shit, we detect error!
		LOG4CXX_WARN(logger, this->imUnitLibraryName << " caught ResultException: " << e.what() << ", result=" << e.getReturnValue());
	    return e.getReturnValue();
	} catch (std::exception &e) {
		// Oh shit, we detect an std error!
		LOG4CXX_WARN(logger, this->imUnitLibraryName << " caught std::exception: " << e.what());
	    return TNC_RESULT_FATAL;
    } catch (...) {
		// Detect unkwnown error!
		LOG4CXX_WARN(logger, this->imUnitLibraryName << " UNKNOWN ERROR");
	    return TNC_RESULT_FATAL;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void IMVLibrary::checkBatchEnding(TNC_IMVID imvID, TNC_ConnectionID connectionID) throw (ResultException)
{
	checkIMUnitLibraryIsInitialized();
	checkIMUnitID(imvID);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result IMVLibrary::batchEnding(TNC_IMVID imvID, TNC_ConnectionID connectionID)
{
	LOG4CXX_DEBUG(logger, this->imUnitLibraryName << "::batchEnding(" << imvID << ", " << connectionID << ")");
    TNC_Result result = TNC_RESULT_FATAL;
    AbstractIMV * imv = NULL;
    try {
    	// check preconditions
    	checkBatchEnding(imvID, connectionID);
    	imv = getImvInstances(connectionID);
    	result = imv->batchEnding();
	} catch (ResultException& e) {
		// Oh shit, we detect error!
		LOG4CXX_WARN(logger, this->imUnitLibraryName << " caught ResultException: " << e.what() << ", result=" << e.getReturnValue());
        result = e.getReturnValue();
	} catch (std::exception &e) {
		// Oh shit, we detect an std error!
		LOG4CXX_WARN(logger, this->imUnitLibraryName << " caught std::exception: " << e.what());
        result = TNC_RESULT_FATAL;
    } catch (...) {
		// Detect unkwnown error!
    	LOG4CXX_WARN(logger, this->imUnitLibraryName << " UNKNOWN ERROR");
        result = TNC_RESULT_FATAL;
    }
    // increase round counter if imv exists
    if(imv)
    	imv->round++;

    return result;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void IMVLibrary::checkTerminate(TNC_IMVID imvID) throw (ResultException)
{
	checkIMUnitLibraryIsInitialized();
	checkIMUnitID(imvID);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result IMVLibrary::terminate(TNC_IMVID imvID)
{
	LOG4CXX_DEBUG(logger, this->imUnitLibraryName << "::terminate(" << imvID << ")");
	try {
		// check preconditions
		checkTerminate(imvID);

		std::map<TNC_ConnectionID, AbstractIMV *>::iterator iter = this->imvInstances.begin();
		// Delete all concrete IMV instances
		while (iter!=this->imvInstances.end())
		{
			delete iter->second;
			iter->second = NULL;
			++iter;
		}

		// Erase map
		this->imvInstances.clear();

		// remove the tncsFunctionPointers
		this->tncsFunctionPointers.bindFunctionPointer          = NULL;
		this->tncsFunctionPointers.provideRecommendationPointer = NULL;
		this->tncsFunctionPointers.reportMessageTypesPointer    = NULL;
		this->tncsFunctionPointers.requestHandshakeRetryPointer = NULL;
		this->tncsFunctionPointers.sendMessagePointer           = NULL;

		// remove optional TNCS function pointers
		this->tncsFunctionPointers.getAttributePointer          = NULL;
		this->tncsFunctionPointers.setAttributePointer          = NULL;

	    // reset imvlibrary
	    this->isIMUnitLibraryInitialized = false;

	    return TNC_RESULT_SUCCESS;
	} catch (ResultException& e) {
		// Oh shit, we detect error!
		LOG4CXX_WARN(logger, this->imUnitLibraryName << " caught ResultException: " << e.what() << ", result=" << e.getReturnValue());
	    return e.getReturnValue();
	} catch (std::exception &e) {
		// Oh shit, we detect an std error!
		LOG4CXX_WARN(logger, this->imUnitLibraryName << " caught std::exception: " << e.what());
	    return TNC_RESULT_FATAL;
    } catch (...) {
		// Detect unkwnown error!
		LOG4CXX_WARN(logger, this->imUnitLibraryName << " UNKNOWN ERROR");
	    return TNC_RESULT_FATAL;
    }
}

} // namespace iml

} // namespace tncfhh
