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
 
#include "IMVProperties.h"
#include "string_helper.h"

#include <log4cxx/logger.h>
#include <iomanip>

namespace tncfhh {

namespace iel {

/* for logging */
static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("TNCS.IMVProperties"));

/* supported min/max versions */
#define TNC_IFIMV_MAX_VERSION TNC_IFIMV_VERSION_1
#define TNC_IFIMV_MIN_VERSION TNC_IFIMV_VERSION_1

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
IMVProperties::IMVProperties(TNC_IMVID id, std::string name, std::string file)
	:id(id), name(name), file(file), initializePointer(NULL), notifyConnectionChangePointer(NULL),
	receiveMessagePointer(NULL), solicitRecommendationPointer(NULL),
	batchEndingPointer(NULL), terminatePointer(NULL), provideBindFunctionPointer(NULL),
	receiveMessageSOHPointer(NULL), receiveMessageLongPointer(NULL), hasFatalError(false)
{
	LOG4CXX_DEBUG(logger, "Create IMVProperties " << id << ":\"" << name << "\" file:\"" << file + "\"");

	// initialize libtool
	int libtoolInit = lt_dlinit();
	// check for errors
	if(libtoolInit)
		throw std::runtime_error(std::string("Error while initializing libtool. ") + lt_dlerror());

	// open libIMV.so
	this->handle = lt_dlopen(file.c_str());
	// check for errors
	if(!this->handle)
		throw std::runtime_error(std::string("Error while loading ") + file + ". " + lt_dlerror());


	/* retrieve function pointers */

	/* get mandatory function pointers */
	this->initializePointer = (TNC_IMV_InitializePointer) getSymbol(this->handle, TNC_IMV_INITIALIZE_NAME);
	this->solicitRecommendationPointer = (TNC_IMV_SolicitRecommendationPointer) getSymbol(this->handle, TNC_IMV_SOLICIT_RECOMMENDATION_NAME);
	this->provideBindFunctionPointer = (TNC_IMV_ProvideBindFunctionPointer) getSymbol(this->handle, TNC_IMV_PROVIDE_BIND_FUNCTION_NAME);

	/* get optional TNC_IMV_NotifyConnectionChangePointer */
	try { this->notifyConnectionChangePointer = (TNC_IMV_NotifyConnectionChangePointer) getSymbol(this->handle, TNC_IMV_NOTIFY_CONNECTION_CHANGE_NAME); }
	catch (std::runtime_error & e) { LOG4CXX_DEBUG(logger, "Optional function not implemented: " << e.what()); }

	/* get optional TNC_IMV_ReceiveMessagePointer */
	try{ this->receiveMessagePointer = (TNC_IMV_ReceiveMessagePointer) getSymbol(this->handle, TNC_IMV_RECEIVE_MESSAGE_NAME); }
	catch (std::runtime_error & e) { LOG4CXX_DEBUG(logger, "Optional function not implemented: " << e.what()); }

	/* get optional TNC_IMV_ReceiveMessageSOHPointer */
	try{ this->receiveMessageSOHPointer = (TNC_IMV_ReceiveMessageSOHPointer) getSymbol(this->handle, TNC_IMV_RECEIVE_MESSAGE_SOH_NAME); }
	catch (std::runtime_error & e) { LOG4CXX_DEBUG(logger, "Optional function not implemented: " << e.what()); }

	/* get optional TNC_IMV_ReceiveMessageLongPointer */
	try{ this->receiveMessageLongPointer = (TNC_IMV_ReceiveMessageLongPointer) getSymbol(this->handle, TNC_IMV_RECEIVE_MESSAGE_LONG_NAME); }
	catch (std::runtime_error & e) { LOG4CXX_DEBUG(logger, "Optional function not implemented: " << e.what()); }

	/* get optional TNC_IMV_BatchEndingPointer */
	try{ this->batchEndingPointer = (TNC_IMV_BatchEndingPointer) getSymbol(this->handle, TNC_IMV_BATCH_ENDING_NAME); }
	catch (std::runtime_error & e) { LOG4CXX_DEBUG(logger, "Optional function not implemented: " << e.what()); }

	/* get optional TNC_IMV_TerminatePointer */
	try{ this->terminatePointer = (TNC_IMV_TerminatePointer) getSymbol(this->handle, TNC_IMV_TERMINATE_NAME); }
	catch (std::runtime_error & e) { LOG4CXX_DEBUG(logger, "Optional function not implemented: " << e.what()); }


	/* initialize libIMV.so */
	call_TNC_IMV_Initialize();

	LOG4CXX_INFO(logger, this->name << " initialized complete.");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *                      IMVProperties::~IMVProperties()                      *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
IMVProperties::~IMVProperties()
{
	LOG4CXX_TRACE(logger, "destructor for IMV " << name);

	this->handleFatalError();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void IMVProperties::call_TNC_IMV_ProvideBindFunction()
{
	/* check if this IMVProperties has FATAL error*/
	if(this->hasFatalError)
	{
		LOG4CXX_WARN(logger, "Error before TNC_IMV_ProvideBindFunction. IMV " << this->name << " has fatal error!");
		return;
	}

	LOG4CXX_TRACE(logger, "Call TNC_IMV_ProvideBindFunction by " << name);

	TNC_Result resultPBF = this->provideBindFunctionPointer(id, TNC_TNCS_BindFunction);
	if (resultPBF != TNC_RESULT_SUCCESS)
	{
		LOG4CXX_WARN(logger, "IMV \"" << name << "\" " << this-> id <<
				             " has permanent error. Function TNC_IMV_ProvideBindFunction returned " << resultPBF);
		this->handleFatalError();
		return;
	}
}

/* methods called by IMV instances*/
TNC_Result IMVProperties::call_TNC_IMV_NotifyConnectionChange(TNC_ConnectionID connectionID,
	                                     TNC_ConnectionState newState)
{
	/* check if this IMVProperties has FATAL error*/
	if(this->hasFatalError)
	{
		LOG4CXX_WARN(logger, "Error before TNC_IMV_NotifyConnectionChange. IMV " << this->name << " has fatal error!");
		return TNC_RESULT_FATAL;
	}

	/* check if .so has notifyConnectionChange function (optional) */
	if(!this->notifyConnectionChangePointer)
	{
		LOG4CXX_WARN(logger, "Error before TNC_IMV_NotifyConnectionChange for IMV " << this->name << " - function not implemented.");
		return TNC_RESULT_OTHER;
	}

	LOG4CXX_TRACE(logger, "Call TNC_IMV_NotifyConnectionChange for \"" << this->name << "\" " << this-> id);
	TNC_Result ret = this->notifyConnectionChangePointer(this->id, connectionID, newState);
	/* check error */
    switch(ret)
    {
		case TNC_RESULT_NOT_INITIALIZED:
			LOG4CXX_INFO(logger, "IMV " << "\"" << this->name << "\" " << this-> id << " not initialized. Trying to initialize now ...");
			call_TNC_IMV_Initialize();
			return call_TNC_IMV_NotifyConnectionChange(connectionID, newState); // FIXME - endless loop
			break;
		case TNC_RESULT_FATAL:
			LOG4CXX_WARN(logger, "IMV \"" << name << "\" " << this-> id << " has permanent error");
			this->handleFatalError();
			return TNC_RESULT_FATAL;
			break;
        default:
    		return ret;
        	break;
    }
}

TNC_Result IMVProperties::call_TNC_IMV_ReceiveMessage(TNC_ConnectionID connectionID,
	                             TNC_BufferReference message,
	                             TNC_UInt32 messageLength,
	                             TNC_MessageType messageType)
{
	/* check if this IMVProperties has FATAL error*/
	if(this->hasFatalError)
	{
		LOG4CXX_INFO(logger, "Error before TNC_IMV_ReceiveMessage. IMV " << this->name << " has fatal error!");
		return TNC_RESULT_FATAL;
	}

	/* check if .so has receiveMessage function (optional) */
	if(!this->receiveMessagePointer)
	{
		LOG4CXX_WARN(logger, "Error before TNC_IMV_ReceiveMessage for IMV " << this->name << " - function not implemented.");
		return TNC_RESULT_OTHER;
	}

	LOG4CXX_TRACE(logger, "Call TNC_IMV_ReceiveMessage for \"" << this->name << "\" " << this-> id);
	TNC_Result ret = this->receiveMessagePointer(this->id, connectionID, message, messageLength, messageType);
	/* check error */
    switch(ret)
    {
		case TNC_RESULT_NOT_INITIALIZED:
			LOG4CXX_INFO(logger, "IMV " << "\"" << this->name << "\" " << this-> id << " not initialized. Trying to initialize now ...");
			call_TNC_IMV_Initialize();
			return call_TNC_IMV_ReceiveMessage(connectionID, message, messageLength, messageType); // FIXME - endless loop
			break;
		case TNC_RESULT_FATAL:
			LOG4CXX_WARN(logger, "IMV \"" << name << "\" " << this-> id << " has permanent error");
			this->handleFatalError();
			return TNC_RESULT_FATAL;
			break;
        default:
    		return ret;
        	break;
    }
}

TNC_Result IMVProperties::call_TNC_IMV_ReceiveMessageSOH(TNC_ConnectionID connectionID,
		                            TNC_BufferReference sohrReportEntry,
									TNC_UInt32 sohrRELength,
									TNC_MessageType systemHealthID)
{
	/* check if this IMVProperties has FATAL error*/
	if(this->hasFatalError)
	{
		LOG4CXX_INFO(logger, "Error before TNC_IMV_ReceiveMessageSOH. IMV " << this->name << " has fatal error!");
		return TNC_RESULT_FATAL;;
	}

	LOG4CXX_WARN(logger, "TNC_IMV_ReceiveMessageSOH not supported yet.");
	return TNC_RESULT_OTHER;
}

TNC_Result IMVProperties::call_TNC_IMV_ReceiveMessageLong(TNC_ConnectionID connectionID,
		                             TNC_UInt32 messageFlags,
		                             TNC_BufferReference message,
		                             TNC_UInt32 messageLength,
		                             TNC_VendorID messageVendorID,
		                             TNC_UInt32 messageSubtype,
		                             TNC_UInt32 sourceIMCID,
		                             TNC_UInt32 destinationIMVID)
{
	/* check if this IMVProperties has FATAL error*/
	if(this->hasFatalError)
	{
		LOG4CXX_INFO(logger, "Error before TNC_IMV_ReceiveMessageLong. IMV " << this->name << " has fatal error!");
		return TNC_RESULT_FATAL;;
	}

	LOG4CXX_WARN(logger, "TNC_IMV_ReceiveMessageLong not supported yet.");
	return TNC_RESULT_OTHER;
}

TNC_Result IMVProperties::call_TNC_IMV_SolicitRecommendation(TNC_ConnectionID connectionID)
{
	/* check if this IMVProperties has FATAL error*/
	if(this->hasFatalError)
	{
		LOG4CXX_INFO(logger, "Error before TNC_IMV_SolicitRecommendation. IMV " << this->name << " has fatal error!");
		return TNC_RESULT_FATAL;;
	}

	LOG4CXX_TRACE(logger, "Call TNC_IMV_SolicitRecommendation for \"" << this->name << "\" " << this-> id);
	TNC_Result ret = this->solicitRecommendationPointer(this->id, connectionID);
	/* check error */
    switch(ret)
    {
		case TNC_RESULT_NOT_INITIALIZED:
			LOG4CXX_INFO(logger, "IMV " << "\"" << this->name << "\" " << this-> id << " not initialized. Trying to initialize now ...");
			call_TNC_IMV_Initialize();
			return call_TNC_IMV_SolicitRecommendation(connectionID); // FIXME - endless loop
			break;
		case TNC_RESULT_FATAL:
			LOG4CXX_WARN(logger, "IMV \"" << name << "\" " << this-> id << " has permanent error");
			this->handleFatalError();
			return TNC_RESULT_FATAL;
			break;
        default:
    		return ret;
        	break;
    }
}

TNC_Result IMVProperties::call_TNC_IMV_BatchEnding(TNC_ConnectionID connectionID)
{
	/* check if this IMVProperties has FATAL error*/
	if(this->hasFatalError)
	{
		LOG4CXX_INFO(logger, "Error before TNC_IMV_BatchEnding. IMV " << this->name << " has fatal error!");
		return TNC_RESULT_FATAL;;
	}

	/* check if .so has batchEnding function (optional) */
	if(!this->batchEndingPointer)
	{
		LOG4CXX_WARN(logger, "Error before TNC_IMV_BatchEnding for IMV " << this->name << " - function not implemented.");
		return TNC_RESULT_OTHER;
	}

	LOG4CXX_TRACE(logger, "Call TNC_IMV_BatchEnding for \"" << this->name << "\" " << this-> id);
	TNC_Result ret = this->batchEndingPointer(this->id, connectionID);
	/* check error */
    switch(ret)
    {
		case TNC_RESULT_NOT_INITIALIZED:
			LOG4CXX_INFO(logger, "IMV " << "\"" << this->name << "\" " << this-> id << " not initialized. Trying to initialize now ...");
			call_TNC_IMV_Initialize();
			return call_TNC_IMV_BatchEnding(connectionID); // FIXME - endless loop
			break;
		case TNC_RESULT_FATAL:
			LOG4CXX_WARN(logger, "IMV \"" << name << "\" " << this-> id << " has permanent error");
			this->handleFatalError();
			return TNC_RESULT_FATAL;
			break;
        default:
    		return ret;
        	break;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *                       TNC_IMVID IMVProperties::getID()                    *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_IMVID IMVProperties::getID() const
{
	return this->id;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *                    std::string IMVProperties::getName()                   *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::string IMVProperties::getName() const
{
	return this->name;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *                     std::string IMVProperties::getFile()                  *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::string IMVProperties::getFile() const
{
	return this->file;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *              IMVProperties::hasMessageType(TNC_MessageType)               *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool IMVProperties::hasMessageType(TNC_MessageType messageType) const
{
	std::list<TNC_MessageType>::const_iterator iter = this->messageTypes.begin();

    while (iter!=this->messageTypes.end())
    {
    	TNC_MessageType localMessageType = *iter;
    	TNC_VendorID localVendorId = (localMessageType >> 8);
    	TNC_MessageSubtype localMessageSubtype = (localMessageType & 0xFF);

    	TNC_VendorID vendorId = (messageType >> 8);
    	TNC_MessageSubtype messageSubtype = (messageType & 0xFF);

    	bool isSameVendorId       = localVendorId==0xFFFFFF   || localVendorId==vendorId;
    	bool isSameMessageSubType = localMessageSubtype==0xFF || localMessageSubtype==messageSubtype;

    	if (isSameVendorId && isSameMessageSubType)
    		return true;

    	iter++;
    }

    return false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *    IMVProperties::reportMessageTypes(TNC_MessageTypeList, TNC_UInt32)     *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void IMVProperties::reportMessageTypes(TNC_MessageTypeList supportedTypes, TNC_UInt32 typeCount)
{
	this->messageTypes.clear();

	for (unsigned int i = 0; i < typeCount; ++i)
	{
		LOG4CXX_TRACE(logger, "Added message type 0x" << std::setw(8) << std::setfill('0') << std::hex << supportedTypes[i]
		                       << " for IMV " << this->name);
		this->messageTypes.push_back(supportedTypes[i]);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
lt_ptr IMVProperties::getSymbol(lt_dlhandle handle, const char *name) throw (std::runtime_error)
{
	lt_ptr ret = lt_dlsym(handle, name);
	if(ret == NULL){
		throw std::runtime_error(std::string("Failed to resolve symbol ") + name + ":" + lt_dlerror());
	}
	return ret;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void IMVProperties::call_TNC_IMV_Initialize()
{
	/* check if this IMVProperties has FATAL error*/
	if(this->hasFatalError)
	{
		LOG4CXX_INFO(logger, "Error before TNC_IMV_Initialize. IMV " << this->name << " has fatal error!");
		return;
	}

	LOG4CXX_TRACE(logger, "Call TNC_IMV_Initialize for \"" << this->name << "\" " << this-> id);
	TNC_Result initResult = this->initializePointer(this->id, TNC_IFIMV_MIN_VERSION, TNC_IFIMV_MAX_VERSION, &(this->ifimvVersion));

	/* check error */
    switch(initResult)
    {
		case TNC_RESULT_ALREADY_INITIALIZED:
			LOG4CXX_INFO(logger, "IMV " << "\"" << this->name << "\" " << this-> id << " already initialized.");
		case TNC_RESULT_SUCCESS:
			break;
        case TNC_RESULT_NO_COMMON_VERSION:
			LOG4CXX_INFO(logger, "IMV " << "\"" << this->name << "\" " << this-> id << "is not compatible with this TNCS.");
        default:
    		LOG4CXX_WARN(logger, "IMV \"" << name << "\" " << this-> id << " has permanent error");
    		this->handleFatalError();
    		return;
        	break;
    }

	/* check if version set correctly by IMV */
	if(this->ifimvVersion > TNC_IFIMV_MAX_VERSION || this->ifimvVersion < TNC_IFIMV_MIN_VERSION)
	{
		LOG4CXX_WARN(logger, "IMV " << name << "\" " << this-> id <<
				" returned success but IMV is not compatible with this TNCS. " <<
				this->ifimvVersion);
		this->handleFatalError();
	}

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void IMVProperties::call_TNC_IMV_Terminate()
{
	/* check if this IMVProperties has FATAL error*/
	if(this->hasFatalError)
	{
		LOG4CXX_INFO(logger, "Error before TNC_IMV_Terminate. IMV " << this->name << " has fatal error!");
		return;
	}

	/* check if .so has terminatePointer function (optional) */
	if(!this->terminatePointer)
	{
		LOG4CXX_WARN(logger, "Error before TNC_IMV_Terminate for IMV " << this->name << " - function not implemented.");
		return;
	}

	/* call TNC_IMV_Terminate */
	LOG4CXX_TRACE(logger, "Call TNC_IMV_Terminate for \"" << this->name << "\" " << this-> id);
	TNC_Result ret = this->terminatePointer(this->id);
	if(ret != TNC_RESULT_SUCCESS)
        LOG4CXX_INFO(logger, "IMV " << "\"" << this->name << "\" " << this-> id << " fatal error TNC_IMV_Terminate! Error is ignored!");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void IMVProperties::handleFatalError()
{
	if (this->hasFatalError) {
		LOG4CXX_TRACE(logger, "Handle fatal error called twice all more often.")
		return;
	}

    // try to call TNC_IMV_Terminate, ignore any errors
	call_TNC_IMV_Terminate();

	// set fatal error flag
	this->hasFatalError = true;

	// reset this instance
	this->initializePointer = NULL;
	this->notifyConnectionChangePointer = NULL;
	this->provideBindFunctionPointer = NULL;
    this->receiveMessageLongPointer = NULL;
    this->receiveMessagePointer = NULL;
    this->receiveMessageSOHPointer = NULL;
    this->solicitRecommendationPointer = NULL;
    this->terminatePointer = NULL;
    this->batchEndingPointer = NULL;

    LOG4CXX_INFO(logger, "Try to unload IMV " << this->name);
    // shutdown libIMV.so
    int libtoolClose = lt_dlclose(this->handle);

    if(libtoolClose)
        LOG4CXX_ERROR(logger, "Error while unloading module. " << lt_dlerror());

//    // shutdown libtool
//    int libtoolExit = lt_dlexit();
//
//    // check for errors
//    if(libtoolExit)
//        LOG4CXX_ERROR(logger, "Error while shutting down libtool. " << lt_dlerror());
}

} // namespace iel

} // namespace tncfhh
