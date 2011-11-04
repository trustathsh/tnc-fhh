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

#include <list>
#include <log4cxx/logger.h>
#include <exception>
#include <iostream>
#include <fstream>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/helpers/exception.h>

namespace tncfhh {

namespace iel {

/* for logging */
static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("TNCS.Coordinator"));

/**
 * C++ Singleton. Definition.
 */
Coordinator coordinator;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Coordinator::Coordinator()
:imvIdCounter(0), conIdCounter(0), isInit(false)
{
    bool log4cxxIsInit = false;

    // Set up a simple configuration that logs on the console.
	// only when no configuration exists
    try {
        log4cxx::LoggerPtr rootLogger = log4cxx::Logger::getRootLogger();
        log4cxxIsInit = rootLogger->getAllAppenders().size() ? true : false;
    } catch ( log4cxx::helpers::Exception& e ) {
        std::cerr << "Log4cxx Exception " <<  e.what() << std::endl;
    }
	if (!log4cxxIsInit)
	{
	    log4cxx::BasicConfigurator::configure();
	}

	LOG4CXX_DEBUG(logger, "Loading tncs version " << TNCFHH_VERSION_MAJOR << "." << TNCFHH_VERSION_MINOR << "." << TNCFHH_VERSION_PATCH);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Coordinator::initialize() throw (std::domain_error)
{
	this->initialize(TNCS_CONFIG);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Coordinator::initialize(std::string file) throw (std::domain_error)
{
	if (this->isInit)
		throw std::domain_error("Coordinator is already initalized.");
	this->isInit = true;

	LOG4CXX_INFO(logger, "Initialize tncs version " << TNCFHH_VERSION_MAJOR << "." << TNCFHH_VERSION_MINOR << "." << TNCFHH_VERSION_PATCH << " ...");

	// initialize all IMVProperties
	// parse config file
    std::vector<std::string> imvs;
	try {
		imvs = parseTncConfigFile(file);
		// retrieve IMV name - .so-path pairs
		for(unsigned int i = 0; i < imvs.size(); i += 2)
		{
			try
			{
				this->imvProperties[imvIdCounter] = NULL;
				this->imvProperties[imvIdCounter] = new IMVProperties(imvIdCounter, imvs.at(i), imvs.at(i+1));
				this->imvProperties[imvIdCounter]->call_TNC_IMV_ProvideBindFunction(); // necessary here for call back
				imvIdCounter++;
			}
			catch (std::exception &e)
			{
				LOG4CXX_WARN(logger, e.what());
				delete this->imvProperties[imvIdCounter];
				this->imvProperties.erase(imvIdCounter);
			}
		}
		LOG4CXX_INFO(logger, "... Initialize tncs done.");
	}
	catch (std::runtime_error &e)
	{
		LOG4CXX_WARN(logger, "Error on initiate tncs: " << e.what());
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Coordinator::~Coordinator()
{
	//nothing
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Coordinator::terminate()
{
	LOG4CXX_TRACE(logger, "Terminate the Coordinator");

	/* delete all Connections */
	std::map<TNC_ConnectionID, Connection *>::iterator iterCon;
	for(iterCon = this->connections.begin(); iterCon != this->connections.end(); ++iterCon)
	{
		delete iterCon->second;
		iterCon->second = NULL;
	}
	this->connections.clear();
	
	/* delete all IMVProperties */
	std::map<TNC_IMVID, IMVProperties *>::iterator iterImvProp;
	for(iterImvProp = this->imvProperties.begin(); iterImvProp != this->imvProperties.end(); ++iterImvProp )
	{
		delete iterImvProp->second;
		iterImvProp->second = NULL;
	}
	this->imvProperties.clear();

	this->isInit = false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_ConnectionState Coordinator::getConnectionState(TNC_ConnectionID connectionID)
{
	try
	{
		// look up Connection. return value can not be NULL.
		Connection *tmpCon = getConnectionById(connectionID);
		return tmpCon->getConnectionState();
	}
	catch (std::runtime_error &e)
	{
		LOG4CXX_WARN(logger, e.what());
		throw ResultException(e.what(), TNC_RESULT_OTHER);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_ConnectionID Coordinator::getConnection(std::string input)
{
	//TODO - Begin critical area
	LOG4CXX_DEBUG(logger, "incoming connection");
	TNC_ConnectionID tmpConId = 0;

	// get connection ID value
	std::map<std::string, TNC_ConnectionID>::iterator iter = this->stringToConId.find(input);
	if(iter == this->stringToConId.end())
	{
		// get new connection value and save it in map
		tmpConId = this->conIdCounter++;
		this->stringToConId[input] = tmpConId;
	} else {
		// con value already available
		tmpConId = iter->second;
	}

	if (this->connections.find(tmpConId) == this->connections.end()) {
		/* create new connection */
		LOG4CXX_DEBUG(logger, "create a new connection " << tmpConId);

		// build list with all IMVproperties for connection ctor
		std::list<IMVProperties *> listIMVsProperties;
		std::map<TNC_IMVID, IMVProperties *>::const_iterator iter = this->imvProperties.begin();
	    while (iter != this->imvProperties.end())
	    {
	    	listIMVsProperties.push_back(iter->second);
	    	iter++;
	    }
//
	    // create new connection instance
		this->connections[tmpConId] = new Connection(tmpConId, listIMVsProperties);
	} else {
		/* found connection */
		LOG4CXX_DEBUG(logger, "found Connection with ID " << tmpConId);
	}

	// get the connection
	Connection * con = this->connections[tmpConId];

	// set new connection status = handshake
	// FIXME - calling notifyConnectionChangehandshake here good?
	con->notifyConnectionChangeHandshake();

	//TODO - End critical area
	return tmpConId;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNCCSData Coordinator::processTNCCSData(TNC_ConnectionID conID, const TNCCSData & receivedData) throw (ConnectionStateException, ConnectionStateExceptionWithTNCCSData, std::logic_error)
{
	//TODO - Begin critical area
	LOG4CXX_DEBUG(logger, "incomming TNCCS Data. cid: " << conID << " length: " << receivedData.getLength());

	if (this->connections.find(conID) == this->connections.end())
	{
		LOG4CXX_WARN(logger, "No Connection for ID " << conID);
		/* no found connection */
		throw std::logic_error(std::string("Coordinator No Connection for ID ") + conID);
	} else
		/* found connection */
		LOG4CXX_TRACE(logger, "Take Connection with connection ID " << conID);

	// get connection
	Connection * con = this->connections[conID];

	//TODO - Begin critical area

	// pass received data to connection and return new batch for the peer
	return con->processTNCCSData(receivedData);
}

/* * * * * * * * * * * * * * TNCS functions * * * * * * * * * * * * * * * * */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Coordinator::reportMessageTypes(TNC_IMVID imvID, TNC_MessageTypeList supportedTypes, TNC_UInt32 typeCount)
{
	LOG4CXX_TRACE(logger, "reportMessageTypes by IMV " << imvID);

	try
	{
		// look up IMVProperties. return value can not be NULL.
		IMVProperties *tmpImvProp = getIMVPropertiesById(imvID);
		tmpImvProp->reportMessageTypes(supportedTypes, typeCount);
	}
	catch (std::runtime_error &e)
	{
		LOG4CXX_WARN(logger, e.what());
		throw ResultException(e.what(), TNC_RESULT_OTHER);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Coordinator::sendMessage(TNC_IMVID imvID, TNC_ConnectionID connectionID, TNC_BufferReference message, TNC_UInt32 messageLength, TNC_MessageType messageType)
{
	LOG4CXX_TRACE(logger, "sendMessage by IMV " << imvID);
	try
	{
		// look up IMVProperties. return value can not be NULL.
		/*IMVProperties *tmpImvProp = */getIMVPropertiesById(imvID);
		// look up Connection. return value can not be NULL.
		Connection *tmpCon = getConnectionById(connectionID);

		tmpCon->sendMessage(imvID, message, messageLength, messageType);
	}
	catch (std::runtime_error &e)
	{
		LOG4CXX_WARN(logger, e.what());
		throw ResultException(e.what(), TNC_RESULT_OTHER);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Coordinator::requestHandshakeRetry(TNC_IMVID imvID, TNC_ConnectionID connectionID, TNC_RetryReason reason)
{
	LOG4CXX_TRACE(logger, "requestHandshakeRetry by IMV " << imvID);
	try
	{
		// look up IMVProperties. return value can not be NULL.
		/*IMVProperties *tmpImvProp = */getIMVPropertiesById(imvID);
		// look up Connection. return value can not be NULL.
		Connection *tmpCon = getConnectionById(connectionID);

		tmpCon->requestHandshakeRetry(imvID, reason);
	}
	catch (std::runtime_error &e)
	{
		LOG4CXX_WARN(logger, e.what());
		throw ResultException(e.what(), TNC_RESULT_OTHER);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Coordinator::provideRecommendation(TNC_IMVID imvID, TNC_ConnectionID connectionID, TNC_IMV_Action_Recommendation recommendation, TNC_IMV_Evaluation_Result evaluation)
{
	LOG4CXX_TRACE(logger, "provideRecommendation by IMV " << imvID);
	try
	{
		// look up IMVProperties. return value can not be NULL.
		/*IMVProperties *tmpImvProp = */getIMVPropertiesById(imvID);
		// look up Connection. return value can not be NULL.
		Connection *tmpCon = getConnectionById(connectionID);

		tmpCon->provideRecommendation(imvID, recommendation, evaluation);
	}
	catch (std::runtime_error &e)
	{
		LOG4CXX_WARN(logger, e.what());
		throw ResultException(e.what(), TNC_RESULT_OTHER);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Coordinator::getAttribute(TNC_IMVID imvID, TNC_ConnectionID connectionID, TNC_AttributeID attributeID, TNC_UInt32 bufferLength, TNC_BufferReference buffer, TNC_UInt32 *pOutValueLength)
{
	LOG4CXX_TRACE(logger, "getAttribute by IMV " << imvID);
	try
	{
		// look up IMVProperties. return value can not be NULL.
		/*IMVProperties *tmpImvProp = */getIMVPropertiesById(imvID);
		// look up Connection. return value can not be NULL.
		Connection *tmpCon = getConnectionById(connectionID);

		tmpCon->getAttribute(imvID, attributeID, bufferLength, buffer, pOutValueLength);
	}
	catch (std::runtime_error &e)
	{
		LOG4CXX_WARN(logger, e.what());
		throw ResultException(e.what(), TNC_RESULT_OTHER);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        void Coordinator::setAttribute(TNC_IMVID, TNC_ConnectionID,        *
 *             TNC_AttributeID, TNC_UInt32, TNC_BufferReference)             *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Coordinator::setAttribute(TNC_IMVID imvID, TNC_ConnectionID connectionID, TNC_AttributeID attributeID, TNC_UInt32 bufferLength, TNC_BufferReference buffer)
{
	LOG4CXX_TRACE(logger, "setAttribute by IMV " << imvID);
	try
	{
		// look up IMVProperties. return value can not be NULL.
		/*IMVProperties *tmpImvProp = */getIMVPropertiesById(imvID);
		// look up Connection. return value can not be NULL.
		Connection *tmpCon = getConnectionById(connectionID);

		tmpCon->setAttribute(imvID, attributeID, bufferLength, buffer);
	}
	catch (std::runtime_error &e)
	{
		LOG4CXX_WARN(logger, e.what());
		throw ResultException(e.what(), TNC_RESULT_OTHER);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
IMVProperties *Coordinator::getIMVPropertiesById(TNC_IMVID imvID) throw (ResultException)
{
	std::map<TNC_IMVID, IMVProperties*>::iterator iter = this->imvProperties.find(imvID);
	// check imvID
	if(iter == this->imvProperties.end())
	{
		throw ResultException(std::string("No IMVProperties for imvID == ") + imvID, TNC_RESULT_INVALID_PARAMETER);
	}

	// check pointer != NULL
	if(iter->second == NULL)
	{
		throw ResultException(std::string("IMVProperties for imvID ") + imvID + " == NULL!", TNC_RESULT_FATAL);
	}

	return iter->second;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Connection *Coordinator::getConnectionById(TNC_ConnectionID conID) throw (std::runtime_error)
{
	std::map<TNC_ConnectionID, Connection *>::iterator iter = this->connections.find(conID);

	// check conID
	if(iter == this->connections.end())
	{
		throw std::runtime_error(std::string("No Connection for conID ") + conID);
	}
	// check pointer != NULL
	if(iter->second == NULL)
	{
		throw std::runtime_error(std::string("Connection for conID ") + conID + " found but connection instance is NULL.");
	}
	return iter->second;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::vector<std::string> Coordinator::parseTncConfigFile(std::string file) throw (std::runtime_error)
{
	LOG4CXX_TRACE(logger, "parsing tnc config file : " << file);
	std::ifstream tnc_config(file.c_str());
	unsigned int maxLineLength = 512;
	char line[maxLineLength];
	std::vector<std::string> ret;
	std::vector<std::string> lines;

	if(!tnc_config)
		throw std::runtime_error(std::string("Could not open tnc config file ") + file);

	// fill all lines to vector
	while(!tnc_config.eof())
	{
		tnc_config.getline(line, maxLineLength);
		if(line != NULL)
			lines.push_back(std::string(line));
	}

	// close file
	tnc_config.close();

	// evaluate all lines
	for(std::vector<std::string>::iterator the_iterator = lines.begin();
	    the_iterator != lines.end();
	    the_iterator++){
		// skip comments
		if(!the_iterator->compare(0, 1, "#")){
//			log.debug("Skipping comment: " + *the_iterator);
		}
		// skip empty lines
		else if(the_iterator->size() == 0){
//			log.debug("Skipping empty line.");
		}
		// process IMV entry
		else if(!the_iterator->compare(0, 4, "IMV ")){
			// find first "
			std::string::size_type start = the_iterator->find( '"');
			// find second "
			std::string::size_type end = the_iterator->rfind( '"');
			// get substring between first " and second "
			std::string imvName = the_iterator->substr(start + 1, end - start - 1);
			// get SO path
			std::string imvSO = the_iterator->substr(end + 2);
			ret.push_back(imvName);
			ret.push_back(imvSO);
			LOG4CXX_TRACE(logger, "Adding IMV from config file: imvName = " << imvName << ", imvSO = " << imvSO);
		}
		// skip invalid lines
		else if(!the_iterator->compare(0, 4, "IMC ")){
//			log.debug("Skipping IMC line.");
		}
		else {
			LOG4CXX_WARN(logger, "Invalid line in " << file << ": " << *the_iterator);
		}
	}
	return ret;
}

}  // namespace iel

}  // namespace tncfhh

