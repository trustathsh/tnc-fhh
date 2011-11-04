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
#include "Connection.h"
#include <log4cxx/logger.h>
#include <sstream>

// supported TNCCS protocols
//#include "soh/SOH1Protocol.h"
#include "tnc1/TNC1Protocol.h"

namespace tncfhh {

namespace iel {

/* for logging */
static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("TNCS.Connection"));

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Connection::Connection(TNC_ConnectionID conID, const std::list<IMVProperties*> &imvProperties)
	:conID(conID), username(NULL), connectionState(TNC_CONNECTION_STATE_CREATE), activeTnccsProtocolHandler(NULL)
{
	LOG4CXX_TRACE(logger, "New Connection " << conID);

	/* create IMVs from IMVProperties */
	std::list<IMVProperties*>::const_iterator iter = imvProperties.begin();
    while (iter!=imvProperties.end())
    {
    	if (!*iter)
    	{
    		/* ignore NULL */
    		LOG4CXX_WARN(logger, "Uhoh strange things happening...imvProperties == NULL");
    		iter++;
    		continue;
    	}

    	LOG4CXX_DEBUG(logger, "Create an IMV " << (*iter)->getName() << " for connection " << conID);
    	// create one new IMV instance per property
    	this->imvInstances[(*iter)->getID()] = new IMV(conID, *iter);

    	iter++;
    }

    // initialize handler chain

//    // SoH
//    AbstractTNCCSProtocol *soh1Protocol = new soh::SOH1Protocol(this->imvInstances);
//    this->tnccsProtocolHandlerChain.push_back(soh1Protocol);

    // 1.x
    AbstractTNCCSProtocol *tnccs1Protocol = new tnc1::TNC1Protocol(this->imvInstances);
//    tnccs1Protocol->setIMVs(this->imvInstances);
    this->tnccsProtocolHandlerChain.push_back(tnccs1Protocol);

    // 2.0
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Connection::~Connection()
{
	LOG4CXX_TRACE(logger, "Delete Connection " << conID);

	/* delete all IMVs */
	for (std::map<TNC_IMVID, IMV*>::iterator iter = this->imvInstances.begin(); iter != this->imvInstances.end(); ++iter)
	{
		delete iter->second;
		iter->second = NULL;
	}

	/* delete all TNCCS protocol handlers */
	for (std::list<AbstractTNCCSProtocol *>::iterator iter = this->tnccsProtocolHandlerChain.begin(); iter != this->tnccsProtocolHandlerChain.end(); ++iter)
	{
		delete *iter;
	}

	this->activeTnccsProtocolHandler = NULL;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Connection::notifyConnectionChangeHandshake()
{
    this->connectionState = TNC_CONNECTION_STATE_HANDSHAKE;

    // inform all IMV about new Handshake
    for (std::map<TNC_IMVID, IMV*>::iterator iter = this->imvInstances.begin();
         iter != this->imvInstances.end();
         ++iter) {
    	iter->second->notifyConnectionChange(TNC_CONNECTION_STATE_HANDSHAKE);
    }

    /* inform all TNCCSProtocols about new Handshake */
    for (std::list<AbstractTNCCSProtocol *>::iterator iter = this->tnccsProtocolHandlerChain.begin();
             iter != this->tnccsProtocolHandlerChain.end();
             ++iter)
    {
    	(*iter)->notifyConnectionChangeHandshake();
    }

    this->activeTnccsProtocolHandler = NULL;

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNCCSData Connection::processTNCCSData(const TNCCSData & receivedData) throw (ConnectionStateException, ConnectionStateExceptionWithTNCCSData, TNCCSProtocolNotSupportedException)
{
	LOG4CXX_TRACE(logger, "processing incoming TNCCSData");
	TNCCSData outgoingTnccsData(0);

	// check if handshake is over
	if (this->connectionState!=TNC_CONNECTION_STATE_HANDSHAKE)
	    throw ConnectionStateException(this->connectionState);

	try
	{
		// first incoming message
		if(!this->activeTnccsProtocolHandler)
		{
			// iterate over all handlers until first one parses successfully
			// save active handler for further processing
			for(std::list<AbstractTNCCSProtocol *>::iterator iter = this->tnccsProtocolHandlerChain.begin();
					iter != this->tnccsProtocolHandlerChain.end();
					++iter)
			{
				try
				{
					this->activeTnccsProtocolHandler = (*iter);
					outgoingTnccsData = (*iter)->processTNCCSData(receivedData);
					break;
				}
				catch (TNCCSProtocolNotSupportedException &e)
				{
					LOG4CXX_TRACE(logger, "Handler does not support IF-TNCCS protocol " << e.what());
					this->activeTnccsProtocolHandler = NULL;
				}
			}
			// check if no compatible handler was found
			if(!this->activeTnccsProtocolHandler)
			{
				LOG4CXX_WARN(logger, "No handler supports requested TNCCS protocol.");
				throw TNCCSProtocolNotSupportedException("No handler supports requested TNCCS protocol.");
			}
		} else {
			// not the first incoming message
			outgoingTnccsData = this->activeTnccsProtocolHandler->processTNCCSData(receivedData);
		}
	}
	// handshake is about to finish + last message
	catch (ConnectionStateExceptionWithTNCCSData &e)
	{
		// save connection state
		this->connectionState = e.getConnectionState();
		// forward exception to inform NAL about finished handshake
		throw e;
	}
	// handshake is about to finish
	catch (ConnectionStateException &e)
	{
		// save connection state
		this->connectionState = e.getConnectionState();
		// forward exception to inform NAL about finished handshake
		throw e;
	}
	// return TNCCSData
	return outgoingTnccsData;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
IMV *Connection::getImvById(TNC_IMVID imvID) throw (std::runtime_error)
{
	LOG4CXX_TRACE(logger, "getImvById(" << imvID << ")");
	std::stringstream error;

	// search IMV by ID
	std::map<TNC_IMVID, IMV*>::iterator iter = this->imvInstances.find(imvID);

	// check if imv exists
	if(iter == this->imvInstances.end())
	{
		error << "No IMV found for ID " << imvID;
		throw std::runtime_error(error.str());
	}
	// check pointer != NULL
	if(iter->second == NULL)
	{
		error << "IMV found for ID " << imvID << " but IMV instance is NULL.";
		throw std::runtime_error(error.str());
	}

	return iter->second;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Connection::sendMessage(TNC_IMVID imvID, TNC_BufferReference message,
		TNC_UInt32 messageLength, TNC_MessageType messageType) throw (ResultException)
{
	LOG4CXX_TRACE(logger, "sendMessage is called: imvID = " << imvID << ". Forwarding it to TNCCSProtocol.");

	try
	{
		// look up IMV. return value can not be NULL.
		IMV *tmpImv = getImvById(imvID);

		// check if receiveMessage | batchEnding has been called for imvID
		if(!tmpImv->isInReceiveMessage() && !tmpImv->isInBatchEnding())
		{
			throw ResultException("sendMessage called in wrong state", TNC_RESULT_ILLEGAL_OPERATION);
		}

		// forward message to TNCCS protocol
		this->activeTnccsProtocolHandler->sendMessage(imvID, message, messageLength, messageType);
	}
	catch (std::runtime_error &e)
	{
		LOG4CXX_WARN(logger, e.what());
		throw ResultException("sendMessage called in wrong state", TNC_RESULT_OTHER);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Connection::requestHandshakeRetry(TNC_IMVID imvID, TNC_RetryReason reason) throw (ResultException)
{
	LOG4CXX_TRACE(logger, "requestHandshakeRetry is called: imvID = " << imvID << ". Function currently not implemented.");
	// TODO requestHandshakeRetry
	throw ResultException("requestHandshakeRetry not implemented", TNC_RESULT_CANT_RETRY);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Connection::provideRecommendation(TNC_IMVID imvID,
		TNC_IMV_Action_Recommendation recommendation, TNC_IMV_Evaluation_Result evaluation)  throw (ResultException)
{
	LOG4CXX_TRACE(logger, "provideRecommendation is called: imvID = " << imvID);

	try
	{
		// look up IMV. return value can not be NULL.
		IMV *tmpImv = getImvById(imvID);
		// set recommendation
		tmpImv->setProvideRecommendation(recommendation, evaluation);
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
void Connection::getAttribute(TNC_IMVID imvID, TNC_AttributeID attributeID, TNC_UInt32 bufferLength, TNC_BufferReference buffer, TNC_UInt32 *pOutValueLength)  throw (ResultException)
{
	LOG4CXX_TRACE(logger, "getAttribute is called: imvID = " << imvID << ".");
//	throw ResultException("getAttribute not implemented", TNC_RESULT_OTHER);

	bool attributeNotFound = true;

	if (attributeID == TNC_ATTRIBUTEID_USERNAME) {
		if (username != NULL) {
			LOG4CXX_TRACE(logger, "Stored username != NULL");

			TNC_UInt32 usernameLength = strlen((const char*) username);

			LOG4CXX_TRACE(logger, "usernameLength: " << usernameLength);

			*pOutValueLength = usernameLength;

			if (bufferLength != 0 && usernameLength <= bufferLength) {
				//buffer = new TNC_Buffer[usernameLength];
				memcpy(buffer, username, usernameLength);
				buffer[usernameLength] = 0;
			}

			attributeNotFound = false;
		} else {
			LOG4CXX_TRACE(logger, "Stored username == NULL");
		}
	}

	if (attributeNotFound) {
		throw ResultException(std::string("No Attribute found for Attribute ID== "/* + attributeID*/), TNC_RESULT_INVALID_PARAMETER);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Connection::setAttribute(TNC_IMVID imvID, TNC_AttributeID attributeID, TNC_UInt32 bufferLength, TNC_BufferReference buffer)  throw (ResultException)
{
	LOG4CXX_TRACE(logger, "setAttribute is called: imvID = " << imvID << ".");
	//throw ResultException("setAttribute not implemented", TNC_RESULT_OTHER);

	bool attributeFound = false;
	bool attributeNotSupported = true;

	if (imvID == 0 && attributeID == TNC_ATTRIBUTEID_USERNAME) {
		LOG4CXX_DEBUG(logger, "imvID is zero and attributeID is TNC_ATTRIBUTEID_USERNAME");
		LOG4CXX_DEBUG(logger, "bufferLength is " << bufferLength);
		username = new TNC_Buffer[bufferLength + 1];
		memcpy(username, buffer, bufferLength);
		username[bufferLength] = '\0';

		attributeFound = true;
		attributeNotSupported = false;
	}

	if (attributeID == TNC_ATTRIBUTEID_PREFERRED_LANGUAGE ||
			attributeID == TNC_ATTRIBUTEID_REASON_LANGUAGE ||
			attributeID == TNC_ATTRIBUTEID_REASON_STRING) {
		attributeFound = true;
	}

	if (attributeNotSupported) {
		throw ResultException(std::string("Setting the value is not supported for Attribute ID== " + attributeID), TNC_RESULT_INVALID_PARAMETER);
	}

	if (!attributeFound) {
		throw ResultException(std::string("No Attribute found for Attribute ID== " + attributeID), TNC_RESULT_INVALID_PARAMETER);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_ConnectionState Connection::getConnectionState()
{
	return this->connectionState;
}

} // namespace iel

} // namespace tncfhh
