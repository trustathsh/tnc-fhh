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
 
#ifndef TNCS_COORDINATOR_H_
#define TNCS_COORDINATOR_H_

#include "string_helper.h"
#include "TNCCSData.h"
#include <vector>
#include "../exception/ConnectionStateException.h"
#include "../exception/ConnectionStateExceptionWithTNCCSData.h"
#include "../exception/ResultException.h"

#include "Connection.h"

#include <tncs/tncsConfig.h>
#include <tncfhhConfig.h>

namespace tncfhh {

namespace iel {

/**
 * TODO_Comment
 * 
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>25.02.2008 - create class (mbs)</li>
 *   <li>26.02.2008 - added tnc_config parsing (ib)</li>
 *   <li>28.02.2008 - remove flowControls in dtor (mbs)</li>
 *   <li>28.02.2008 - added TNCS functions (ib)</li>
 *   <li>11.04.2008 - added connectionCreate (mbs)</li>
 *   <li>14.05.2008 - update error handling (mbs)</li>
 *   <li>16.05.2008 - can change tnc_config file by compiler (mbs)</li>
 *   <li>11.07.2009 - redesign for 0.6.0 (mbs,ib)</li>
 * </ul>
 *
 * @class Coordinator
 * @brief TODO_Brief
 * @date 25.02.2008
 * @author Mike Steinmetz (mbs)
 * @author Ingo Bente (ib)
 */
class Coordinator
{
public:
	
	/**
	 * Constructor. Just one instance is created, when this library is
	 * loaded.
	 */
	Coordinator();

	/**
	 * Destructor
	 */
	virtual ~Coordinator();
	
	/**
	 * TODO_Comment
	 *
	 * @param conID the connection ID
	 */
	virtual TNC_ConnectionID getConnection(std::string input);
	
	/**
	 * TODO_Comment
	 *
	 * @param conID The connectionID for which receivedBatch was received.
	 * @param receivedData The received data from the peer.
	 *
	 * @return Data that shall be sent to the peer in response.
	 */
	virtual TNCCSData processTNCCSData(TNC_ConnectionID conID, const TNCCSData & receivedData) throw (ConnectionStateException, ConnectionStateExceptionWithTNCCSData, std::logic_error);
	
	// TNCS functions
	
	/**
	 * This function delegates the reportMessageTypes() call to the specific IMVProperties.
	 * 
	 * @param imvID
	 * @param supportedTypes
	 * @param typeCount
	 * 
	 * @throw TNCUtil::exception::ResultException
	 * 
	 * @see TNC_TNCS_ReportMessageTypes()
	 */
	void reportMessageTypes(TNC_IMVID imvID, TNC_MessageTypeList supportedTypes, TNC_UInt32 typeCount);
	
	/**
	 * This function delegates the sendMessage() call to the connectionID specific FlowControl.
	 * 
	 * @param imvID
	 * @param connectionID
	 * @param message
	 * @param messageLength
	 * @param messageType
	 * 
	 * @throw TNCUtil::exception::ResultException
	 * 
	 * @see TNC_TNCS_SendMessage()
	 */
	void sendMessage(TNC_IMVID imvID, TNC_ConnectionID connectionID, TNC_BufferReference message, TNC_UInt32 messageLength, TNC_MessageType messageType);
	
	/**
	 * This function delegates the requestHandshakeRetry() call to the connectionID specific FlowControl.
	 * 
	 * @param imvID
	 * @param connectionID
	 * @param reason
	 * 
	 * @throw TNCUtil::exception::ResultException
	 * 
	 * @see TNC_TNCS_RequestHandshakeRetry()
	 */
	void requestHandshakeRetry(TNC_IMVID imvID, TNC_ConnectionID connectionID, TNC_RetryReason reason);
	
	/**
	 * This function delegates the provideRecommendation() call to the connectionID specific FlowControl.
	 * 
	 * @param imvID
	 * @param connectionID
	 * @param recommendation
	 * @param evaluation
	 * 
	 * @throw TNCUtil::exception::ResultException
	 * 
	 * @see TNC_TNCS_ProvideRecommendation()
	 */
	void provideRecommendation(TNC_IMVID imvID, TNC_ConnectionID connectionID, TNC_IMV_Action_Recommendation recommendation, TNC_IMV_Evaluation_Result evaluation);
	
	/**
	 * This function delegates the getAttribute() call to the connectionID specific FlowControl.
	 * 
	 * @param imvID
	 * @param connectionID
	 * @param attributeID
	 * @param bufferLength
	 * @param buffer [out]
	 * @param pOutValueLength [out]
	 * 
	 * @throw TNCUtil::exception::ResultException
	 * 
	 * @see TNC_TNCS_GetAttribute()
	 */
	void getAttribute(TNC_IMVID imvID, TNC_ConnectionID connectionID, TNC_AttributeID attributeID, TNC_UInt32 bufferLength, /*out*/ TNC_BufferReference buffer, /*out*/ TNC_UInt32 *pOutValueLength);
	
	/**
	 * This function delegates the setAttribute() call to the connectionID specific FlowControl.
	 * 
	 * @param imvID
	 * @param connectionID
	 * @param attributeID
	 * @param bufferLength
	 * @param buffer
	 * 
	 * @throw TNCUtil::exception::ResultException
	 * 
	 * @see TNC_TNCS_SetAttribute()
	 */
	void setAttribute(TNC_IMVID imvID, TNC_ConnectionID connectionID, TNC_AttributeID attributeID, TNC_UInt32 bufferLength, TNC_BufferReference buffer);
	
	TNC_ConnectionState getConnectionState(TNC_ConnectionID connectionID);
	
	/*
	 * Innitiate Coordinator (parse config file etc.).
	 *
	 * @param file the tnc config file
	 *
	 * @throw std::domain_error if already initiate
	 */
	void initialize(std::string file) throw (std::domain_error);

	/*
	 * Initiate Coordinator (parse config file etc.).
	 *
	 * @throw std::domain_error if already initiate
	 */
	void initialize() throw (std::domain_error);

	/*
	 * Terminate Coordinator (remove all conection and IMVs).
	 *
	 */
	void terminate();

private:
	
	/**
	 * TODO_Comment
	 *
	 * @param enclosing_method_arguments
	 *
	 * @return return_type
	 */
	TNC_UInt32 imvIdCounter;

	TNC_UInt32 conIdCounter;

	bool isInit;

	std::map<std::string, TNC_ConnectionID> stringToConId;

	/**
	 * TODO_Comment
	 *
	 * @param enclosing_method_arguments
	 *
	 * @return return_type
	 */
	std::map<TNC_ConnectionID, Connection *> connections;

	/**
	 * TODO_Comment
	 */
	std::map<TNC_IMVID, IMVProperties *> imvProperties;

	/**
	 * Parses the tnc config file (TNC_CONFIG_FILE).
	 */
	std::vector<std::string> parseTncConfigFile(std::string file) throw (std::runtime_error);

	/**
	 * TODO_Comment
	 *
	 * @param enclosing_method_arguments
	 *
	 * @return return_type
	 */
	IMVProperties *getIMVPropertiesById(TNC_IMVID imvID) throw (ResultException);
		
	/**
	 * TODO_Comment
	 *
	 */
	Connection *getConnectionById(TNC_ConnectionID conID) throw (std::runtime_error);
};

/**
 * C++ Singleton. Declaration.
 */
extern Coordinator coordinator;

}  // namespace iel

}  // namespace tncfhh

#endif /*TNCS_COORDINATOR_H_*/
