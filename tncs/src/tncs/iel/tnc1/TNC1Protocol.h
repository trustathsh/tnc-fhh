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
#ifndef TNCS_TNC1PROTOCOL_H_
#define TNCS_TNC1PROTOCOL_H_

#include "../AbstractTNCCSProtocol.h"
#include "TNCCSBatch.h"

namespace tncfhh {

namespace iel {

namespace tnc1 {

/**
 * TODO Comment
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>13.07.2009 - create class (mbs)</li>
 * </ul>
 *
 * @class TNC1Protocol
 * @brief TODO Brief
 * @date 13.07.2009
 * @author Mike Steinmetz (mbs)
 */
class TNC1Protocol: public tncfhh::iel::AbstractTNCCSProtocol {
public:
	/*
	 * Default-Ctor.
	 */
	TNC1Protocol(const std::map<TNC_IMVID, IMV*> &imvsMap);

	/*
	 * Default-Dtor.
	 */
	virtual ~TNC1Protocol();

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
	virtual void sendMessage(TNC_IMVID imvID, TNC_BufferReference message, TNC_UInt32 messageLength, TNC_MessageType messageType);

	/**
	 * This function delegates the requestHandshakeRetry() call to the connectionID specific FlowControl.
	 *
	 * @param imvID
	 * @param reason
	 *
	 * @throw TNCUtil::exception::ResultException
	 *
	 * @see TNC_TNCS_RequestHandshakeRetry()
	 */
	virtual void requestHandshakeRetry(TNC_IMVID imvID, TNC_RetryReason reason);

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
	virtual void getAttribute(TNC_IMVID imvID, TNC_AttributeID attributeID, TNC_UInt32 bufferLength, /*out*/ TNC_BufferReference buffer, /*out*/ TNC_UInt32 *pOutValueLength);

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
	virtual void setAttribute(TNC_IMVID imvID, TNC_AttributeID attributeID, TNC_UInt32 bufferLength, TNC_BufferReference buffer);

	/*
	 * @param incomingTNCCSData
	 *
	 * @return outgoingTNCCSData
	 *
	 * @throw ConnectionStateException, TNCCSProtocolNotSupportedException
	 */
	virtual TNCCSData processTNCCSData(TNCCSData incomingTNCCSData) throw (ConnectionStateException, TNCCSProtocolNotSupportedException);

	/**
	 * Informs this protocol instance about a new handshake. Does <b>not</b> call
	 * TNC_IMV_notifyConnectionChange
	 */
	virtual void notifyConnectionChangeHandshake();

private:
	TNCCSBatch incoming;
	TNCCSBatch outgoing;

	/*
	 * If true then wait for the 1. TNCCSBatch (see notifyConnectionChangeHandshake())
	 */
	bool isFirst;

	void checkIMVID(TNC_IMVID imvID, std::map<TNC_IMVID, IMV *>::iterator &iter) throw (std::domain_error);

	bool doXACMLEvaluation(std::list<IMV*> imvs, TNC_IMV_Action_Recommendation &recommendation);
};

}

}

}

#endif /*TNCS_ TNC1PROTOCOL_H_ */
