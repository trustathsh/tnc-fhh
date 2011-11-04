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
 
#include "IMUnitLibrary.h"
#include <string.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <stdlib.h>

// include log4cxx header files.
#include "log4cxx/logger.h"
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/propertyconfigurator.h"
#include "log4cxx/helpers/exception.h"

using namespace log4cxx;
using namespace log4cxx::helpers;

namespace tncfhh {

namespace iml {

static LoggerPtr logger(Logger::getLogger("IMUnit.IMUnitLibrary"));

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
IMUnitLibrary::IMUnitLibrary()
	:messageTypeList(NULL), typeCount(0), imUnitLibraryName(""),
	isIMUnitLibraryInitialized(false), imUnitLibraryID(0),
	ifImcImvVersion(1/*TNC_IFIMC_VERSION_1 or TNC_IFIMV_VERSION_1*/)
{
    // Set up a simple configuration that logs on the console.
	// only when no configuration exists
	if (!checkLog4cxxInitialized())
	{
		// try to use config file
		const char* log4cxxConfigurationEnv = getenv("LOG4CXX_CONFIGURATION");

		if (!log4cxxConfigurationEnv)
		{
			log4cxxConfigurationEnv = "log4cxx.properties";
		}

		PropertyConfigurator::configure(log4cxxConfigurationEnv);
		if(!checkLog4cxxInitialized())
		{
			BasicConfigurator::configure();
			LOG4CXX_DEBUG(logger, "no log4cxx configuration file. using basic configuration");
		}
	}
	LOG4CXX_INFO(logger, "Load imunit-Library version " << TNCFHH_VERSION_MAJOR << "." << TNCFHH_VERSION_MINOR << "." << TNCFHH_VERSION_PATCH);

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
IMUnitLibrary::~IMUnitLibrary()
{
	delete[] this->messageTypeList;
	this->messageTypeList = NULL;
	this->typeCount = 0;

	LOG4CXX_INFO(logger, "Unload Library " << this->getIMUnitLibraryName());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_MessageTypeList IMUnitLibrary::getMessageTypeList()
{
	return this->messageTypeList;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_UInt32 IMUnitLibrary::getMessageTypeCount()
{
	return this->typeCount;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::string IMUnitLibrary::getIMUnitLibraryName()
{
	return this->imUnitLibraryName;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool IMUnitLibrary::getIMUnitLibraryInitialized()
{
	return this->isIMUnitLibraryInitialized;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_IMUnitID IMUnitLibrary::getIMUnitLibraryID()
{
	return this->imUnitLibraryID;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Version IMUnitLibrary::getIfImcImvVersion()
{
	return this->ifImcImvVersion;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void IMUnitLibrary::addMessageType(TNC_MessageType messageType)
{
	// save temporary old list
	TNC_MessageTypeList tmpList = this->messageTypeList;
	// create memory for new list
	this->messageTypeList = new TNC_MessageType[this->typeCount+1];
	// copy old list in new list
	memcpy(this->messageTypeList, tmpList, this->typeCount * sizeof(TNC_MessageType));
	// add new messageType in new list
	this->messageTypeList[this->typeCount] = messageType;
	// update typeCount
	this->typeCount++;
	// delete tmplist
	delete[] tmpList;
	tmpList = NULL;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void IMUnitLibrary::addMessageType(TNC_VendorID vendorId, TNC_MessageSubtype messageSubtype)
{
	this->addMessageType((vendorId << 8) | (messageSubtype & 0xff));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void IMUnitLibrary::setIMUnitLibraryName(std::string &imUnitLibraryName)
{
	this->imUnitLibraryName = imUnitLibraryName;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void IMUnitLibrary::setIMUnitLibraryInitialized(bool initialized)
{
	this->isIMUnitLibraryInitialized = initialized;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void IMUnitLibrary::setIMUnitLibraryID(TNC_IMUnitID id)
{
	this->imUnitLibraryID = id;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void IMUnitLibrary::setIfImcImvVersion(TNC_Version version)
{
	this->ifImcImvVersion = version;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result IMUnitLibrary::initialize(TNC_IMUnitID imUnitID,
		                             TNC_Version minVersion,
		                             TNC_Version maxVersion,
		                             TNC_Version *pOutActualVersion)
{
	LOG4CXX_INFO(logger, this->imUnitLibraryName << "::initialize(" << imUnitID << ", " << minVersion << ", " << maxVersion << ")");
	try {
		// check if initialize is called correctly
		checkInitialize(imUnitID, minVersion, maxVersion, pOutActualVersion);
		this->imUnitLibraryID = imUnitID;
		// IMUnit sets IF-IMC/IMV version to use
		*pOutActualVersion = this->ifImcImvVersion;
		this->isIMUnitLibraryInitialized = true;
		// Everything is OK
		return TNC_RESULT_SUCCESS;
	} catch (ResultException& e) {
		// Oh shit, we detect error!
	    std::cerr << e.what() << std::endl;
	    return e.getReturnValue();
	} catch (std::exception &e) {
	    // Oh shit, we detect an std error!
	    std::cerr << e.what() << std::endl;
		return TNC_RESULT_FATAL;
	} catch (...) {
		// Detect unkwnown error!
	    std::cerr << "UNKOWN ERROR" << std::endl;
	    return TNC_RESULT_FATAL;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void IMUnitLibrary::checkInitialize(TNC_IMUnitID imUnitID,
                                   TNC_Version minVersion,
                                   TNC_Version maxVersion,
                                   TNC_Version *pOutActualVersion) throw (ResultException)
{
	std::stringstream ss;

	// check parameters
    if (minVersion > maxVersion) {
    	ss << "minVersion " << minVersion << " > maxVersion " << maxVersion;
        throw ResultException(ss.str(), TNC_RESULT_INVALID_PARAMETER);
    } else if (this->ifImcImvVersion < minVersion || this->ifImcImvVersion > maxVersion) {
    	ss << "Unsupported IMC/IMV API Versions. minVersion: " << minVersion << " maxVersion: " << maxVersion << " this->ifimcimvversion: " << this->ifImcImvVersion;
        throw ResultException(ss.str(), TNC_RESULT_NO_COMMON_VERSION);
    } else if (pOutActualVersion == NULL) {
    	ss << "The output parameter pOutActualVersion is NULL.";
        throw ResultException(ss.str(), TNC_RESULT_INVALID_PARAMETER);
    }

    // check if library is already initialized
    if(this->isIMUnitLibraryInitialized){
    	ss << "IMUnitLibrary " << this->imUnitLibraryName << "already initialized";
        throw ResultException(ss.str(), TNC_RESULT_ALREADY_INITIALIZED);
    }

    // all is o.k.
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void IMUnitLibrary::checkIMUnitLibraryIsInitialized() throw (ResultException)
{
	if(!this->isIMUnitLibraryInitialized) throw ResultException("IMUnitLibrary not initialized.", TNC_RESULT_NOT_INITIALIZED);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void IMUnitLibrary::checkIMUnitID(TNC_IMUnitID imUnitID) throw (ResultException)
{
	std::stringstream ss;
	if(this->imUnitLibraryID != imUnitID){
		ss << "IMunitIDs do not match: " << this->imUnitLibraryID << " != " << imUnitID;
		throw ResultException(ss.str(), TNC_RESULT_INVALID_PARAMETER);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void IMUnitLibrary::checkReceiveMessage(TNC_IMUnitID id,
		                                TNC_BufferReference messageBuffer,
		                                TNC_UInt32 messageLength,
		                                TNC_MessageType messageType) throw (ResultException)
{
	std::stringstream ss;
	checkIMUnitLibraryIsInitialized();
    checkIMUnitID(id);

    if (messageLength != 0 && messageBuffer == NULL){
    	ss << "messageLength != 0 && messageBuffer == NULL";
    	throw ResultException(ss.str(), TNC_RESULT_INVALID_PARAMETER);
    }

    // check for invalid broadcasts
    if ((0x000000ff & messageType) == 0xff)
    	throw ResultException("MessageSubType == 0xff", TNC_RESULT_INVALID_PARAMETER);
    if ((messageType >> 8) == 0xffffff)
    	throw ResultException("VendorID == 0xffffff", TNC_RESULT_INVALID_PARAMETER);

    // all o.k.
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool IMUnitLibrary::checkLog4cxxInitialized()
{
    bool result = false;

    try {
        LoggerPtr rootLogger = Logger::getRootLogger();
        result = rootLogger->getAllAppenders().size() ? true : false;
    } catch ( log4cxx::helpers::Exception& e ) {
        std::cout << "Log4cxx Exception " <<  e.what() << std::endl;
    }

    return result;
}


}  // namespace iml

}  // namespace tncfhh

