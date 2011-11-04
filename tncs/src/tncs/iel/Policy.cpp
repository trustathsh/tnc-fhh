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
#include "Policy.h"
#include <log4cxx/logger.h>

namespace tncfhh {

namespace iel {

/* for logging */
static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("TNCS.Policy"));

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *                             Policy::Policy()                              *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Policy::Policy()
{
	LOG4CXX_TRACE(logger, "Constructor Policy");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *                             Policy::~Policy()                             *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Policy::~Policy()
{
	LOG4CXX_TRACE(logger, "Destructor Policy");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *                    void Policy::resetPolicy()                             *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Policy::resetPolicy()
{
	/* Do nothing. */ // Todo - Policy::resetPolicy
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *                 Policy::getActionRecommendation()                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_IMV_Action_Recommendation Policy::getActionRecommendation(const std::list<IMV *> &imvs)
{
	LOG4CXX_TRACE(logger, "getActionRecommendation");

	bool accessAllow = true;
	bool accessIsolate = true;

	/* check all IMVs have provided recommendation and compute overall recommendation */
	for (std::list<IMV*>::const_iterator iter = imvs.begin(); iter != imvs.end(); ++iter)
	{
		if (!(*iter)->hasProvideRecommendation())
		{
			// last try to call solicitRecommendation
			LOG4CXX_DEBUG(logger, "IMV " << (*iter)->getIMVProperties()->getName() << " connection " << (*iter)->getConnectionID() << " has not provided recommendation yet. Last try calling solicitRecommendation.");
			(*iter)->solicitRecommendation();
		}

		TNC_IMV_Action_Recommendation imvActionRecommendation;

		if (!(*iter)->hasProvideRecommendation())
		{
			/* WARNING imv has not provided recommendation */
			LOG4CXX_WARN(logger, "IMV " << (*iter)->getIMVProperties()->getName() << " connection " << (*iter)->getConnectionID() << " has ultimately not provided recommendation.");
			imvActionRecommendation = TNC_IMV_ACTION_RECOMMENDATION_NO_RECOMMENDATION;
		} else {
			LOG4CXX_DEBUG(logger, "IMV " << (*iter)->getIMVProperties()->getName() << " connection " << (*iter)->getConnectionID()
					<< " has provided recommendation " << (*iter)->getRecommendation() << " and evaluation " << (*iter)->getEvaluation());
			imvActionRecommendation = (*iter)->getRecommendation();
		}

		// incorporate single recommendation to overall result
		accessAllow &= (imvActionRecommendation == TNC_IMV_ACTION_RECOMMENDATION_ALLOW);
		accessIsolate &= ((imvActionRecommendation == TNC_IMV_ACTION_RECOMMENDATION_ISOLATE) | (imvActionRecommendation == TNC_IMV_ACTION_RECOMMENDATION_ALLOW));
	}

	// special case if there are no IMVs -> access none
	if (imvs.size() == 0)
		accessAllow = accessIsolate = false;

	if (accessAllow)
		return TNC_IMV_ACTION_RECOMMENDATION_ALLOW;

	if (accessIsolate)
		return TNC_IMV_ACTION_RECOMMENDATION_ISOLATE;

	// if (accessNone)
	return TNC_IMV_ACTION_RECOMMENDATION_NO_ACCESS;

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *       void Policy::isHandshakeFinished(TNCCSBatch&, std::list<IMV*>)      *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//void Policy::isHandshakeFinished(TNCCSBatch & outgoingTnccsBatch,  std::list<IMV*> imvs)
//{
//	TNCLog &log = TNCLog::getLog(CLASSNAME, "isHandshakeFinished");
//
//	if (outgoingTnccsBatch.getImcImvMessages().size() == 0)
//		handshakeIsFinished(outgoingTnccsBatch, imvs);
//
//	log.info("Handshake is not finished");
//}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *      void Policy::handshakeIsFinished(TNCCSBatch&, std::list<IMV*>&)      *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//void Policy::handshakeIsFinished(TNCCSBatch & outgoingTnccsBatch, std::list<IMV*> & imvs)
//{
//	TNCLog &log = TNCLog::getLog(CLASSNAME, "handshakeIsFinished");
//
//	outgoingTnccsBatch.deleteAllMessages();
//
//	xercesc::DOMImplementation * impl = xercesc::DOMImplementationRegistry::getDOMImplementation(XercesString("LS").c_str());
//	if (!impl)
//		throw TNCUtil::exception::Exception("Policy::handshakeIsFinished: Xerces error: No DOMImplementation LS found!");
//	xercesc::DOMDocument * doc = impl->createDocument();
//
//	xercesc::DOMElement * xml = doc->createElement(XercesString("TNCCS-Recommendation").c_str());
//	TNC_MessageType messageType = 0x01;
//
//
//	bool accessAllowed = true;
//	bool accessIsolated = true;
//
//	/* check all IMVs has provide recommendation an calculate access */
//	for (std::list<IMV*>::iterator iter = imvs.begin();
//	     iter != imvs.end();
//	     ++iter)
//	{
//		if (!(*iter)->hasProvideRecommendation()) {
//			(*iter)->solicitRecommendation();
//		}
//
//		TNC_IMV_Action_Recommendation imvActionRecommendation;
//
//		if (!(*iter)->hasProvideRecommendation()) {
//			/* WARNING imv has no provide recommendation */
//			log.warn("IMV %s has not provided recommendation", (*iter)->getProperties().getName().c_str());
//			imvActionRecommendation = TNC_IMV_ACTION_RECOMMENDATION_NO_RECOMMENDATION;
//		} else {
//			log.debug("IMV %s has provided recommendation: %lu and evaluation: %lu", (*iter)->getProperties().getName().c_str(), (*iter)->getRecommendation(), (*iter)->getEvaluation());
//			imvActionRecommendation = (*iter)->getRecommendation();
//		}
//
//		accessAllowed  &= imvActionRecommendation==TNC_IMV_ACTION_RECOMMENDATION_ALLOW;
//		accessIsolated &= ((imvActionRecommendation==TNC_IMV_ACTION_RECOMMENDATION_ISOLATE)|(imvActionRecommendation==TNC_IMV_ACTION_RECOMMENDATION_ALLOW));
//	}
//	if (imvs.size() == 0)
//		accessAllowed = accessIsolated = false;
//
//
//	if (accessAllowed) {
//		/* acces allowed */
//		xml->setAttribute(XercesString("type").c_str(), XercesString("allow").c_str());
//
//		outgoingTnccsBatch.addTNCCTNCSMessage(new TNCCTNCSMessageXML(messageType, doc, xml));
//
//	    /* inform all IMV about access allowed */
//	    for (std::list<IMV*>::iterator iter = imvs.begin();
//	         iter != imvs.end();
//	         ++iter) {
//	    	(*iter)->notifyConnectionChange(TNC_CONNECTION_STATE_ACCESS_ALLOWED);
//	    }
//
//	    throw NAATNCS::exception::ConnectionStateException("Access allowed", TNC_CONNECTION_STATE_ACCESS_ALLOWED, outgoingTnccsBatch.getNewNalTnccsBatch());
//	}
//
//
//	if (accessIsolated) {
//		/* acces isolated */
//		xml->setAttribute(XercesString("type").c_str(), XercesString("isolate").c_str());
//
//		outgoingTnccsBatch.addTNCCTNCSMessage(new TNCCTNCSMessageXML(messageType, doc, xml));
//
//	    /* inform all IMV about access isolated */
//	    for (std::list<IMV*>::iterator iter = imvs.begin();
//	         iter != imvs.end();
//	         ++iter) {
//	    	(*iter)->notifyConnectionChange(TNC_CONNECTION_STATE_ACCESS_ISOLATED);
//	    }
//
//		throw NAATNCS::exception::ConnectionStateException("Access isolated", TNC_CONNECTION_STATE_ACCESS_ISOLATED, outgoingTnccsBatch.getNewNalTnccsBatch());
//	}
//
//	/* no acces allowed */
//	xml->setAttribute(XercesString("type").c_str(), XercesString("none").c_str());
//
//	outgoingTnccsBatch.addTNCCTNCSMessage(new TNCCTNCSMessageXML(messageType, doc, xml));
//
//    /* inform all IMV about access none */
//    for (std::list<IMV*>::iterator iter = imvs.begin();
//         iter != imvs.end();
//         ++iter) {
//    	(*iter)->notifyConnectionChange(TNC_CONNECTION_STATE_ACCESS_NONE);
//    }
//
//    throw NAATNCS::exception::ConnectionStateException("Access none", TNC_CONNECTION_STATE_ACCESS_NONE, outgoingTnccsBatch.getNewNalTnccsBatch());
//}

} // namespace iel

} // namespace tncfhh
