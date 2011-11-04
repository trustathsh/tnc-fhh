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
 
#include "IMCLibrary.h"

#include <log4cxx/logger.h>
#include <sstream>
#include <iomanip>

using namespace log4cxx;

namespace tncfhh {

namespace iml {

static LoggerPtr logger(Logger::getLogger("IMUnit.IMUnitLibrary.IMCLibrary"));

IMCLibrary::IMCLibrary()
{
//	LOG4CXX_TRACE(logger, "IMCLibrary()");
}

IMCLibrary::~IMCLibrary()
{
//	LOG4CXX_TRACE(logger, "~IMCLibrary()");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void * IMCLibrary::getTNCCFunctionPointer(const char *functionName, TNC_BindFunctionPointer tncBindFunction) throw (ResultException)
{
	LOG4CXX_TRACE(logger, this->imUnitLibraryName << "::getTNCCFunctionPointer(" << functionName << ", " << tncBindFunction << ")");
    void *tncFunctionPointer = NULL;
    tncBindFunction(this->imUnitLibraryID, const_cast<char*>(functionName), &tncFunctionPointer);
    if (tncFunctionPointer == NULL) {
    	std::stringstream ss;
    	ss << functionName << " not found";
        throw ResultException(ss.str(), TNC_RESULT_FATAL);
    }
    return tncFunctionPointer;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
AbstractIMC * IMCLibrary::getImcInstances(TNC_ConnectionID id) throw (ResultException)
{
	std::map<TNC_ConnectionID, AbstractIMC *>::iterator result = imcInstances.find(id);
	if (result == imcInstances.end())
	{
		std::stringstream ss;
		ss << "No IMC for connection ID " << id << "!";
		throw ResultException(ss.str(), TNC_RESULT_INVALID_PARAMETER);
	}
	return imcInstances[id];
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void IMCLibrary::checkProvideBindFunction(TNC_IMCID imcID, TNC_TNCC_BindFunctionPointer bindFunction) throw (ResultException)
{
	checkIMUnitLibraryIsInitialized();
	checkIMUnitID(imcID);
	if(bindFunction == NULL){
		throw ResultException("Pointer to bindFunction == NULL.", TNC_RESULT_INVALID_PARAMETER);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result IMCLibrary::provideBindFunction(TNC_IMCID imcID, TNC_TNCC_BindFunctionPointer bindFunction)
{
	LOG4CXX_DEBUG(logger, this->imUnitLibraryName << "::provideBindFunction(" << imcID << ", " << bindFunction << ")");
    try {
    	// check preconditions
    	checkProvideBindFunction(imcID, bindFunction);
        // set the tnccFunctionPointers
        this->tnccFunctionPointers.bindFunctionPointer = bindFunction;
        this->tnccFunctionPointers.reportMessageTypesPointer = (TNC_TNCC_ReportMessageTypesPointer) this->getTNCCFunctionPointer("TNC_TNCC_ReportMessageTypes", bindFunction);
        this->tnccFunctionPointers.requestHandshakeRetryPointer = (TNC_TNCC_RequestHandshakeRetryPointer) this->getTNCCFunctionPointer("TNC_TNCC_RequestHandshakeRetry", bindFunction);
        this->tnccFunctionPointers.sendMessagePointer= (TNC_TNCC_SendMessagePointer) this->getTNCCFunctionPointer("TNC_TNCC_SendMessage", bindFunction);
        // report messages types to TNC client
        this->tnccFunctionPointers.reportMessageTypesPointer(this->imUnitLibraryID, this->messageTypeList, this->typeCount);
        // everything is OK
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
void IMCLibrary::checkNotifyConnectionChange(TNC_IMCID imcID, TNC_ConnectionID connectionID, TNC_ConnectionState newState) throw (ResultException)
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
TNC_Result IMCLibrary::notifyConnectionChange(TNC_IMCID imcID, TNC_ConnectionID connectionID, TNC_ConnectionState newState)
{
	LOG4CXX_DEBUG(logger, this->imUnitLibraryName << "::notifyConnectionChange(" << imcID << ", " << connectionID << ", " << newState << ")");
	AbstractIMC * imc = NULL;
	TNC_Result result = TNC_RESULT_FATAL;

	try {
		// check preconditions
		checkNotifyConnectionChange(imcID, connectionID, newState);
		switch (newState) {
			case TNC_CONNECTION_STATE_CREATE:
				{
					/* check imc does not exist */
					bool exist = false;
					try {
						this->getImcInstances(connectionID);
						exist = true;
					} catch (ResultException& e) {
						// expected exception. there should be no IMC for a new connection.
						LOG4CXX_TRACE(logger, "expected exception: No IMC for connection ID " << connectionID << " and connectionState = CREATE");
					}
					// error. IMCs exist for a new connection
					if (exist) throw ResultException("TNC_IMC_NotifyConnectionChange(TNC_CONNECTION_STATE_CREATE): IMC already exist.", TNC_RESULT_ILLEGAL_OPERATION);

					/* Create concrete IMC instance */
					LOG4CXX_TRACE(logger, "Library " << this->imUnitLibraryName << " creating IMC instance for conID " << connectionID);
					AbstractIMC * tmp = this->createNewImcInstance(connectionID);
					if (!tmp)
						throw ResultException("createNewIMCInstance(..) returns NULL", TNC_RESULT_FATAL);

					// save IMC
					imcInstances[connectionID] = tmp;
					imc = getImcInstances(connectionID);
				}
				break;
			case TNC_CONNECTION_STATE_DELETE:
				// Connection will be deleted. Remove IMC instance from map.
				imc = getImcInstances(connectionID);
				imcInstances.erase(connectionID);
				break;
			case TNC_CONNECTION_STATE_ACCESS_ALLOWED:
			case TNC_CONNECTION_STATE_ACCESS_ISOLATED:
			case TNC_CONNECTION_STATE_ACCESS_NONE:
			case TNC_CONNECTION_STATE_HANDSHAKE:
				imc = getImcInstances(connectionID);
				break;
			default:
				throw ResultException("Unknown TNC_ConnectionState!", TNC_RESULT_INVALID_PARAMETER);
			}

		    result = imc->notifyConnectionChange(newState);
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

	    // delete imc if necessary
	    if (newState == TNC_CONNECTION_STATE_DELETE){
	    	LOG4CXX_WARN(logger, this->imUnitLibraryName << " deleting imc for connection ID " << connectionID << " imc ID " << imcID);
	    	delete imc;
	    	imc = NULL;
	    }

	    // reset round counter if imc exists and connection state handshake
	    if(imc && (newState == TNC_CONNECTION_STATE_HANDSHAKE))
	    	imc->round = 0;

	    return result;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void IMCLibrary::checkBeginHandshake(TNC_IMCID imcID, TNC_ConnectionID connectionID) throw (ResultException)
{
	checkIMUnitLibraryIsInitialized();
	checkIMUnitID(imcID);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result IMCLibrary::beginHandshake(TNC_IMCID imcID, TNC_ConnectionID connectionID)
{
	LOG4CXX_DEBUG(logger, this->imUnitLibraryName << "::beginHandshake(" << imcID << ", " << connectionID << ")");
    TNC_Result result = TNC_RESULT_FATAL;
    AbstractIMC *imc = NULL;
    try {
    	// check preconditions
    	checkBeginHandshake(imcID, connectionID);
	    imc = getImcInstances(connectionID);
	    result = imc->beginHandshake();
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
    // increase round counter if imc exists
    if(imc)
    	imc->round++;

    return result;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result IMCLibrary::receiveMessage(TNC_IMCID imcID, TNC_ConnectionID connectionID,
		                              TNC_BufferReference messageBuffer, TNC_UInt32 messageLength,
		                              TNC_MessageType messageType)
{
	LOG4CXX_DEBUG(logger, this->imUnitLibraryName << "::receiveMessage("
			<< "imcID=" << imcID << ", "
			<< "conID=" << connectionID << ", "
			<< "messageBuffer=0x" << std::setw(sizeof(void*)) << std::setfill('0') << std::hex << (void*)messageBuffer << ", "
			<< "messageLength=" << messageLength << ", "
			<< "messageType=0x" << std::setw(8) << std::setfill('0') << std::hex << messageType << ")");

	try {
		checkReceiveMessage(imcID, messageBuffer, messageLength, messageType);
		return getImcInstances(connectionID)->receiveMessage(messageBuffer, messageLength, messageType);
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
void IMCLibrary::checkBatchEnding(TNC_IMCID imcID, TNC_ConnectionID connectionID) throw (ResultException)
{
	checkIMUnitLibraryIsInitialized();
	checkIMUnitID(imcID);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result IMCLibrary::batchEnding(TNC_IMCID imcID, TNC_ConnectionID connectionID)
{
	LOG4CXX_DEBUG(logger, this->imUnitLibraryName << "::batchEnding(" << imcID << ", " << connectionID << ")");
    TNC_Result result = TNC_RESULT_FATAL;
    AbstractIMC *imc = NULL;
    try {
    	// check preconditions
    	checkBatchEnding(imcID, connectionID);
    	imc = getImcInstances(connectionID);
    	result = imc->batchEnding();
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
    // increase round counter if imc exists
    if(imc)
    	imc->round++;

    return result;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void IMCLibrary::checkTerminate(TNC_IMCID imcID) throw (ResultException)
{
	checkIMUnitLibraryIsInitialized();
	checkIMUnitID(imcID);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result IMCLibrary::terminate(TNC_IMCID imcID)
{
	LOG4CXX_DEBUG(logger, this->imUnitLibraryName << "::terminate(" << imcID << ")");
	try {
		// check preconditions
		checkTerminate(imcID);

		std::map<TNC_ConnectionID, tncfhh::iml::AbstractIMC *>::iterator iter = this->imcInstances.begin();

	    // Delete all concrete IMC instances
		while (iter!=this->imcInstances.end()) {
			delete iter->second;
			iter->second = NULL;
			++iter;
		}

		// Erase map
		this->imcInstances.clear();

	    // remove the tnccFunctionPointers
	    this->tnccFunctionPointers.bindFunctionPointer          = NULL;
	    this->tnccFunctionPointers.reportMessageTypesPointer    = NULL;
	    this->tnccFunctionPointers.requestHandshakeRetryPointer = NULL;
	    this->tnccFunctionPointers.sendMessagePointer           = NULL;

	    // reset imclibrary
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
