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
 
#include "SOH1Protocol.h"
#include "../../exception/ConnectionStateException.h"
#include "../../exception/ConnectionStateExceptionWithTNCCSData.h"
#include "../string_helper.h"

#include <log4cxx/logger.h>

namespace tncfhh {

namespace iel {

namespace soh {

/* for logging */
static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("TNCS.TNC1.Protocol"));

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
SOH1Protocol::SOH1Protocol(const std::map<TNC_IMVID, IMV*> &imvsMap)
:AbstractTNCCSProtocol(imvsMap), incoming(TNCCSBatch::TNCS),
outgoing(TNCCSBatch::TNCC)
{
	// Lets rock!
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
SOHProtocol::~SOH1Protocol() {
	// unrockable?!
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void SOH1Protocol::sendMessage(TNC_IMVID imvID, TNC_BufferReference message, TNC_UInt32 messageLength, TNC_MessageType messageType)
{
	// get iterator
	std::map<TNC_IMVID, IMV*>::iterator iter = this->imvs.find(imvID);

	// check imvID
	checkIMVID(imvID, iter);

	// check if receiveMessage | batchEnding has been called for imvID
	if(!iter->second->isInReceiveMessage() && !iter->second->isInBatchEnding()){
		throw std::domain_error(std::string("sendMessage called in wrong state."));
	}

	// TODO call Policy.checkMessage()

	// add message to outgoing batch
	this->outgoing.addIMCIMVMessage(new IMCIMVMessage(message, messageLength, messageType));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void SOH1Protocol::requestHandshakeRetry(TNC_IMVID imvID, TNC_RetryReason reason)
{
	//TODO - TNC1Protocol - requestHandshakeRetry
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void SOH1Protocol::getAttribute(TNC_IMVID imvID, TNC_AttributeID attributeID, TNC_UInt32 bufferLength, /*out*/ TNC_BufferReference buffer, /*out*/ TNC_UInt32 *pOutValueLength)
{
	//TODO - TNC1Protocol - getAttribute
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void SOH1Protocol::setAttribute(TNC_IMVID imvID,  TNC_AttributeID attributeID, TNC_UInt32 bufferLength, TNC_BufferReference buffer)
{
	//TODO - TNC1Protocol - setAttribute
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNCCSData SOH1Protocol::processTNCCSData(TNCCSData incomingTNCCSData) throw (ConnectionStateException, TNCCSProtocolNotSupportedException)
{
	try {
		// convert TNCSBatch to TNCSData and save it in incoming
		this->incoming = incomingTNCCSData;

		// delete old outgoing batch
		this->outgoing.deleteAllMessages();

		std::list<IMCIMVMessage *> imcImvMessages = this->incoming.getImcImvMessages();
		std::list<IMCIMVMessage *>::iterator iterM = imcImvMessages.begin();
		while(iterM!=imcImvMessages.end()){
			IMCIMVMessage * imcImvMessage = *iterM;

			/*  */
			for (std::map<TNC_IMVID, IMV*>::iterator iterI = this->imvs.begin();
				 iterI != this->imvs.end();
				 ++iterI) {

				if ((iterI->second)->getIMVProperties()->hasMessageType(imcImvMessage->getType()))
					(iterI->second)->receiveMessage(imcImvMessage->getData(), imcImvMessage->getDataLength(), imcImvMessage->getType());
			}
			iterM++;
		}

		// inform all IMV about batchEnding
		for (std::map<TNC_IMVID, IMV*>::iterator iter = this->imvs.begin();
			 iter != this->imvs.end();
			 ++iter) {
			iter->second->batchEnding();
		}

		/* create a list with all IMVs */
		std::list<IMV*> imvs;
		for (std::map<TNC_IMVID, IMV*>::iterator iter = this->imvs.begin();
			 iter != this->imvs.end();
			 ++iter) {
			imvs.push_back(iter->second);
		}


		if (this->outgoing.getImcImvMessages().size() == 0) {
			TNC_IMV_Action_Recommendation recommendation = policy.getActionRecommendation(imvs);
			throw ConnectionStateExceptionWithTNCCSData(recommendation, this->outgoing.getNewTnccsData());
		}

		//Connection::exchangeTNCCSBatch return
		return this->outgoing.getNewTnccsData();
	} catch (std::runtime_error & e) {
		LOG4CXX_ERROR(logger, e.what());
		throw ConnectionStateException(TNC_IMV_ACTION_RECOMMENDATION_NO_RECOMMENDATION);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void SOH1Protocol::checkIMVID(TNC_IMVID imvID, std::map<TNC_IMVID, IMV *>::iterator &iter) throw (std::domain_error)
{

	// check if imv exists
	if(iter == this->imvs.end()){
		throw std::domain_error(std::string("No IMV for imvID == ") + imvID);
	}
	// check pointer != NULL
	if(iter->second == NULL){
		throw std::domain_error(std::string("IMV for imvID ") + imvID + " == NULL!");
	}
}



}

}

}
