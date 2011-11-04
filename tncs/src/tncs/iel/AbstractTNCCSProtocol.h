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
#ifndef TNCS_ABSTRACTTNCCSPROTOCOL_H_
#define TNCS_ABSTRACTTNCCSPROTOCOL_H_

#include <tcg/tnc/tncifimv.h>
#include <string>
#include "Policy.h"
#include "TNCCSData.h"
#include <list>
#include <map>
#include "../exception/ConnectionStateException.h"
#include "../exception/TNCCSProtocolNotSupportedException.h"

namespace tncfhh {

namespace iel {

/**
 * TODO Comment
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>13.07.2009 - create class (mbs)</li>
 * </ul>
 *
 * @class AbstractTNCCSProtocol
 * @brief TODO Brief
 * @date 13.07.2009
 * @author Mike Steinmetz (mbs)
 */
class AbstractTNCCSProtocol {
public:
	/*
	 * Default-Ctor.
	 */
	AbstractTNCCSProtocol(const std::map<TNC_IMVID, IMV*> &imvsMap);

	/*
	 * Default-Dtor.
	 */
	virtual ~AbstractTNCCSProtocol();

	/**
	 * This function delegates the sendMessage() call to the connectionID specific FlowControl.
	 *
	 * @param imvID
	 * @param message
	 * @param messageLength
	 * @param messageType
	 *
	 * @throw TNCUtil::exception::ResultException
	 *
	 * @see TNC_TNCS_SendMessage()
	 */
	virtual void sendMessage(TNC_IMVID imvID, TNC_BufferReference message, TNC_UInt32 messageLength, TNC_MessageType messageType)=0;

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
	virtual void requestHandshakeRetry(TNC_IMVID imvID, TNC_RetryReason reason)=0;

	/**
	 * This function delegates the getAttribute() call to the connectionID specific FlowControl.
	 *
	 * @param imvID
	 * @param attributeID
	 * @param bufferLength
	 * @param buffer [out]
	 * @param pOutValueLength [out]
	 *
	 * @throw TNCUtil::exception::ResultException
	 *
	 * @see TNC_TNCS_GetAttribute()
	 */
	virtual void getAttribute(TNC_IMVID imvID, TNC_AttributeID attributeID, TNC_UInt32 bufferLength, /*out*/ TNC_BufferReference buffer, /*out*/ TNC_UInt32 *pOutValueLength)=0;

	/**
	 * This function delegates the setAttribute() call to the connectionID specific FlowControl.
	 *
	 * @param imvID
	 * @param attributeID
	 * @param bufferLength
	 * @param buffer
	 *
	 * @throw TNCUtil::exception::ResultException
	 *
	 * @see TNC_TNCS_SetAttribute()
	 */
	virtual void setAttribute(TNC_IMVID imvID, TNC_AttributeID attributeID, TNC_UInt32 bufferLength, TNC_BufferReference buffer)=0;

	/**
	 * Returns the name of the protocol.
	 */
	std::string getProtocolName() const;

	/**
	 * Sets the IMVs for the TNCCS Protocol instance.
	 * IMVs are needed for calling TNC_IMV functions.
	 */
//	void setIMVs(const std::map<TNC_IMVID, IMV*> &imvsMap);

	/**
	 * Informs this protocol instance about a new handshake. Does <b>not</b> call
	 * TNC_IMV_notifyConnectionChange
	 */
	virtual void notifyConnectionChangeHandshake();

	/**
	 * Processes incoming TNCCSData. If this protocol can process the incoming
	 * data, this will lead normally to a receiveMessage call to the IMV instances.
	 * Those IMVs will then normally call sendMessage which is delegated
	 * over IFTNCSImpl->Coordinator->Connection to this protocol. Those sendMessage
	 * calls will be composed to a outgoing TNCCSData instance which is returned
	 * by this method.
	 */
	virtual TNCCSData processTNCCSData(TNCCSData incomingTNCCSData) throw (ConnectionStateException, TNCCSProtocolNotSupportedException);

protected:
	std::string protocolName;
	Policy policy;
	std::map<TNC_IMVID, IMV*> imvs;
};


} // namespace iel

} // namespace tncfhh

#endif /*TNCS_ ABSTRACTTNCCSPROTOCOL_H_ */
