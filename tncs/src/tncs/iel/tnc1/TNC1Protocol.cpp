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
 
#include "TNC1Protocol.h"
#include "../../exception/ConnectionStateException.h"
#include "../../exception/ConnectionStateExceptionWithTNCCSData.h"
#include "../string_helper.h"
#include "TNCCTNCSMessageXML.h"

#include <tncxacml/XACMLHelper.h>
#include <tncxacml/XACMLConstants.h>

#include <tncutil/Configuration.h>

#include "../Coordinator.h"

#include <log4cxx/logger.h>

namespace tncfhh {

namespace iel {

namespace tnc1 {

/* for logging */
static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("TNCS.TNC1.Protocol"));

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC1Protocol::TNC1Protocol(const std::map<TNC_IMVID, IMV*> &imvsMap)
:AbstractTNCCSProtocol(imvsMap), incoming(TNCCSBatch::TNCS),
outgoing(TNCCSBatch::TNCC), isFirst(true)
{
	// Lets rock!
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC1Protocol::~TNC1Protocol() {
	// unrockable?!
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TNC1Protocol::sendMessage(TNC_IMVID imvID, TNC_BufferReference message, TNC_UInt32 messageLength, TNC_MessageType messageType)
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
void TNC1Protocol::requestHandshakeRetry(TNC_IMVID imvID, TNC_RetryReason reason)
{
	//TODO - TNC1Protocol - requestHandshakeRetry
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TNC1Protocol::getAttribute(TNC_IMVID imvID, TNC_AttributeID attributeID, TNC_UInt32 bufferLength, /*out*/ TNC_BufferReference buffer, /*out*/ TNC_UInt32 *pOutValueLength)
{
	//TODO - TNC1Protocol - getAttribute
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TNC1Protocol::setAttribute(TNC_IMVID imvID,  TNC_AttributeID attributeID, TNC_UInt32 bufferLength, TNC_BufferReference buffer)
{
	//TODO - TNC1Protocol - setAttribute
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNCCSData TNC1Protocol::processTNCCSData(TNCCSData incomingTNCCSData) throw (ConnectionStateException, TNCCSProtocolNotSupportedException)
{
	try {
		// convert TNCCSData to TNCCSBatch and save it in incoming
		this->incoming = incomingTNCCSData;

		if (this->isFirst)
			this->isFirst = false;

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

		/* if no IMC-IMV-Message to send than finished (calculate recommendation) */
		if (this->outgoing.getImcImvMessages().size() == 0) {
			LOG4CXX_TRACE(logger, "No outgoing IMV messages. Retrieving recommendation ...");
			/* add TNCCS message for recommendation */

			/* get THE XercesC implementaion */
			xercesc::DOMImplementation * impl = xercesc::DOMImplementationRegistry::getDOMImplementation(XercesString("LS").c_str());
			if (!impl)
			    throw std::logic_error("TNC1Protocol::processTNCCSData Xerces error: No DOMImplementation LS found!");
			// create a new document
			xercesc::DOMDocument * doc = impl->createDocument();
			// create a new XML element
            xercesc::DOMElement * xml = doc->createElementNS(TNCCSBatch::iftnccsNamespaceURI.c_str(), XercesString("TNCCS-Recommendation").c_str());
            // set xml to root
            doc->appendChild(xml);

            /**
             * Let an XACML PDP evaluate the decision.
             */
            TNC_IMV_Action_Recommendation recommendation;
            bool xacmlSuccessful = doXACMLEvaluation(imvs, recommendation);
            if (xacmlSuccessful) {
            	LOG4CXX_DEBUG(logger, "Evaluation by XACML was successful.");
            // do local evaluation
            } else {
            	LOG4CXX_DEBUG(logger, "Evaluation by XACML was not successful, trying local policy checks.");
            	// calculate the recommendation by means of the policy
            	recommendation = policy.getActionRecommendation(imvs);
            }

			// define TNCCS message type for recommendations (see IF-TNCS specification)
			TNC_MessageType messageType = 0x01;
			// define connection state
			TNC_ConnectionState conState = TNC_CONNECTION_STATE_HANDSHAKE;

			/* witch recommendation return the policy? */
			switch (recommendation) {
				case TNC_IMV_ACTION_RECOMMENDATION_ALLOW:
					/* access allowed */
					xml->setAttribute(XercesString("type").c_str(), XercesString("allow").c_str());
					conState = TNC_CONNECTION_STATE_ACCESS_ALLOWED;
					break;
				case TNC_IMV_ACTION_RECOMMENDATION_ISOLATE:
					/* access isolate */
					xml->setAttribute(XercesString("type").c_str(), XercesString("isolate").c_str());
					conState = TNC_CONNECTION_STATE_ACCESS_ISOLATED;
					break;
				case TNC_IMV_ACTION_RECOMMENDATION_NO_ACCESS:
					/* access none */
					xml->setAttribute(XercesString("type").c_str(), XercesString("none").c_str());
					conState = TNC_CONNECTION_STATE_ACCESS_NONE;
					break;
				case TNC_IMV_ACTION_RECOMMENDATION_NO_RECOMMENDATION:
					/* no recommendation currently handled like access none*/
					xml->setAttribute(XercesString("type").c_str(), XercesString("none").c_str());
					conState = TNC_CONNECTION_STATE_ACCESS_NONE;
					break;
				default:
					/* should never happen, handled as access none */
					LOG4CXX_WARN(logger, "Unknown recommendation " << recommendation << ". Handled as access none");
					xml->setAttribute(XercesString("type").c_str(), XercesString("none").c_str());
					conState = TNC_CONNECTION_STATE_ACCESS_NONE;
					break;
			}
			/* add TNCCS message */
			this->outgoing.addTNCCTNCSMessage(new TNCCTNCSMessageXML(messageType, doc));
			/* inform all IMV about connection state */
			for (std::list<IMV*>::iterator iter = imvs.begin(); iter != imvs.end();++iter) {
				(*iter)->notifyConnectionChange(conState);
			}
			/* throw exception to inform iel / nal */
			throw ConnectionStateExceptionWithTNCCSData(conState, this->outgoing.getNewTnccsData());
		}

		//Connection::exchangeTNCCSBatch return
		return this->outgoing.getNewTnccsData();
	} catch (TNCCSProtocolNotSupportedException & e) {
		/* catch exception if input data contain no/wrong TNC1.x protocol */
		if (this->isFirst) {
			/* this exception it is legitimate for the first message */
			LOG4CXX_INFO(logger, e.what());
			throw;
		} else {
			/* this exception should not arise, deny access for non specific conform client */
			LOG4CXX_WARN(logger, e.what());
			throw ConnectionStateException(TNC_CONNECTION_STATE_ACCESS_NONE);
		}
	} catch (ConnectionStateException) {
		throw;
	} catch (std::exception & e) {
		LOG4CXX_ERROR(logger, e.what());
		throw ConnectionStateException(TNC_CONNECTION_STATE_ACCESS_NONE);
	} catch (...) {
		LOG4CXX_ERROR(logger, "Catch unknown error in processTNCCSData.");
		throw ConnectionStateException(TNC_CONNECTION_STATE_ACCESS_NONE);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TNC1Protocol::checkIMVID(TNC_IMVID imvID, std::map<TNC_IMVID, IMV *>::iterator &iter) throw (std::domain_error)
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TNC1Protocol::notifyConnectionChangeHandshake()
{
	LOG4CXX_TRACE(logger, "Notify connection change handshake, reset TNC 1.x")

	/* reset batch IDs */
	this->incoming.resetID();
	this->outgoing.resetID();

	this->isFirst = true;
}

bool TNC1Protocol::doXACMLEvaluation(std::list<IMV*> imvs, TNC_IMV_Action_Recommendation &recommendation)
{
	tncfhh::Configuration* config = tncfhh::Configuration::instance();
	config->addConfigurationFile(std::string("/etc/tnc/xacml/xacml-ifm-imv.conf"));

	std::list<IMV*>::const_iterator iter = imvs.begin();
	TNC_ConnectionID connectionID = (*iter)->getConnectionID();

	TNC_UInt32 usernameBufferLength = 128;
	TNC_BufferReference usernameBuffer = new TNC_Buffer[usernameBufferLength];
	TNC_UInt32* pOutValueLength = (TNC_UInt32*) malloc(sizeof(TNC_UInt32));

	try {
		tncfhh::iel::coordinator.getAttribute(0, connectionID, TNC_ATTRIBUTEID_USERNAME, 128, usernameBuffer, pOutValueLength);
		/**
		 * if the output length is greater than 0,
		 * a username could be read.
		 */
		if (*pOutValueLength != 0) {
			LOG4CXX_DEBUG(logger, "Username for this connection: " << usernameBuffer);

			/**
			 * Create the XACML-Request
			 */
			DOMDocument* request = tncfhh::iml::XACMLHelper::createRequest();

			DOMElement* subjectUsername = tncfhh::iml::XACMLHelper::createSubject(request);
			DOMElement* attributeUsername = tncfhh::iml::XACMLHelper::createAttribute(request, tncfhh::iml::XACMLConstants::attribute_id_subject_id, tncfhh::iml::XACMLConstants::data_type_xml_string, usernameBuffer);
			tncfhh::iml::XACMLHelper::addAttributeToSubject(subjectUsername, attributeUsername);
			tncfhh::iml::XACMLHelper::addSubjectToRequest(request, subjectUsername);

			DOMElement* subjectServername = tncfhh::iml::XACMLHelper::createSubject(request, tncfhh::iml::XACMLConstants::subject_category_tnc_tncs);
			DOMElement* attributeServername = tncfhh::iml::XACMLHelper::createAttribute(request, tncfhh::iml::XACMLConstants::attribute_id_tncs_server_name, tncfhh::iml::XACMLConstants::data_type_xml_string, std::string("TNCS"));
			tncfhh::iml::XACMLHelper::addAttributeToSubject(subjectServername, attributeServername);
			tncfhh::iml::XACMLHelper::addSubjectToRequest(request, subjectServername);

			std::string mapping[4];
			mapping[TNC_IMV_ACTION_RECOMMENDATION_ALLOW] = std::string("ALLOW");
			mapping[TNC_IMV_ACTION_RECOMMENDATION_NO_ACCESS] = std::string("NO_ACCESS");
			mapping[TNC_IMV_ACTION_RECOMMENDATION_ISOLATE] = std::string("ISOLATE");
			mapping[TNC_IMV_ACTION_RECOMMENDATION_NO_RECOMMENDATION] = std::string("NO_RECOMMENDATION");

			const IMVProperties* properties;
			std::string imvName;
			std::string ifmValue;
			DOMElement* subjectIMV;
			DOMElement* attributeIMV;
			TNC_IMV_Action_Recommendation tmpRecommendation;

			subjectIMV = tncfhh::iml::XACMLHelper::createSubject(request, tncfhh::iml::XACMLConstants::subject_category_tnc_tncs);

			for (std::list<IMV*>::iterator iter = imvs.begin(); iter != imvs.end(); iter++) {
				properties = (*iter)->getIMVProperties();
				imvName = properties->getName();
				ifmValue = config->get(imvName);
				tmpRecommendation = (*iter)->getRecommendation();

				attributeIMV = tncfhh::iml::XACMLHelper::createAttribute(request, tncfhh::iml::XACMLConstants::attribute_id_component_recommendation_prefix + ifmValue + tncfhh::iml::XACMLConstants::attribute_id_component_recommendation_postfix, tncfhh::iml::XACMLConstants::data_type_xml_string, mapping[tmpRecommendation]);
				tncfhh::iml::XACMLHelper::addAttributeToSubject(subjectIMV, attributeIMV);

				LOG4CXX_TRACE(logger, "IMV name: " << imvName << ", IF-M value: " + ifmValue + ", recommendation: " + tmpRecommendation);
			}

			tncfhh::iml::XACMLHelper::addSubjectToRequest(request, subjectIMV);
			tncfhh::iml::XACMLHelper::addResourceActionEnvironmentToRequest(request);

			/**
			 * Evaluate the request by an XACML-PDP.
			 */
			recommendation = tncfhh::iml::XACMLHelper::getXACMLRecommendation(request);

			if (recommendation != TNC_IMV_ACTION_RECOMMENDATION_NO_RECOMMENDATION) {
				if (recommendation == TNC_IMV_ACTION_RECOMMENDATION_ALLOW) {
					LOG4CXX_DEBUG(logger, "XACML evaluation resulted in 'ALLOW'");
				} else if (recommendation == TNC_IMV_ACTION_RECOMMENDATION_ISOLATE) {
					LOG4CXX_DEBUG(logger, "XACML evaluation resulted in 'ISOLATE'");
				} else if (recommendation == TNC_IMV_ACTION_RECOMMENDATION_NO_ACCESS) {
					LOG4CXX_DEBUG(logger, "XACML evaluation resulted in 'NO ACCESS'");
				}

				return true;
			} else {
				LOG4CXX_DEBUG(logger, "No evaluation by XACML-PDP.");
				return false;
			}
		} else {
			LOG4CXX_DEBUG(logger, "Username could not be read");
			return false;
		}
	} catch (tncfhh::iel::ResultException &e) {
		LOG4CXX_DEBUG(logger, "Username could not be read");
		return false;
	}
}

}

}

}
