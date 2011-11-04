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
 
#include "XACMLHelper.h"
#include "XACMLConstants.h"
#include <xercesc/framework/MemBufInputSource.hpp>
#include <log4cxx/logger.h>
#include <stdexcept>

#include <tncutil/Configuration.h>

using namespace log4cxx;

namespace tncfhh {

namespace iml {

/* for logging */
static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger(
		"IMUNIT.XACMLHelper"));

XACMLHelper::XercesInit XACMLHelper::xercesInit;

XACMLHelper::XercesInit::XercesInit() {

	try {
		xercesc::XMLPlatformUtils::Initialize();
		LOG4CXX_TRACE(logger, "xerces is initialized.");
	} catch (const xercesc::XMLException& toCatch) {
		char* message = xercesc::XMLString::transcode(toCatch.getMessage());
		LOG4CXX_ERROR(logger, "Error during initialization!: " << message);
		xercesc::XMLString::release(&message);
		throw std::exception();
	}
}

XACMLHelper::XercesInit::~XercesInit() {

	try {
		xercesc::XMLPlatformUtils::Terminate();
		LOG4CXX_TRACE(logger, "xerces terminated.");
	} catch (const xercesc::XMLException& toCatch) {
		char* message = xercesc::XMLString::transcode(toCatch.getMessage());
		LOG4CXX_ERROR(logger, "Error during initialization!: " << message);
		xercesc::XMLString::release(&message);
		throw std::exception();
	}
}

XACMLHelper::XACMLHelper() {

}

XACMLHelper::~XACMLHelper() {
	// TODO Auto-generated destructor stub
}

xercesc::DOMDocument* XACMLHelper::parseIncomingXMLData(
/*in*/std::string incomingString) {
	XercesDOMParser* parser = new XercesDOMParser();

	parser->setValidationScheme(XercesDOMParser::Val_Never);
	parser->setDoNamespaces(false);
	parser->setDoSchema(false);
	parser->setLoadExternalDTD(false);

	MemBufInputSource inputSource((XMLByte*) incomingString.c_str(),
			incomingString.length(), "XML");
	parser->parse(inputSource);
	DOMDocument* response = parser->getDocument();

	return response;
}

TNC_IMV_Action_Recommendation XACMLHelper::parseResponse(
/*in*/xercesc::DOMDocument* response) {
	try {
		DOMElement* root = response->getDocumentElement();
		if (root == 0) {
			LOG4CXX_ERROR(logger, "Empty XML document.");
			return TNC_IMV_ACTION_RECOMMENDATION_NO_RECOMMENDATION;
		}

		DOMNodeList* decisions = response->getElementsByTagName(XercesString(
				"Decision").c_str());
		const XMLSize_t decisionNodeCount = decisions->getLength();
		LOG4CXX_TRACE(logger, "# of decisions: " << decisionNodeCount);

		// For all nodes, children of "root" in the XML tree.
		for (XMLSize_t i = 0; i < decisionNodeCount; ++i) {
			DOMNode* currentDecisionNode = decisions->item(i);

			if (currentDecisionNode->getNodeType() && // true is not NULL
					currentDecisionNode->getNodeType() == DOMNode::ELEMENT_NODE) { // is element node
				// Found node which is an Element. Re-cast node as element
				DOMElement* currentDecisionElement = dynamic_cast<xercesc::DOMElement*> (currentDecisionNode);

				DOMNodeList* decisionElementChildren =
						currentDecisionElement->getChildNodes();
				const XMLSize_t decisionElementChildrenCount =
						decisionElementChildren->getLength();
				LOG4CXX_TRACE(logger, "# of children of decision '" << i
						<< "': " << decisionElementChildrenCount);

				for (XMLSize_t j = 0; j < decisionElementChildrenCount; ++j) {
					DOMNode* currentChild = decisionElementChildren->item(j);

					if (currentChild->getNodeType() && // true is not NULL
							currentChild->getNodeType() == DOMNode::TEXT_NODE) { // is text node
						// Found node which is an Text node. Re-cast node as text
						DOMText* currentText =
								dynamic_cast<xercesc::DOMText*> (currentChild);

						LOG4CXX_DEBUG(logger, "Decision: " << XercesString(
								currentText->getData()).toString());

						if (XMLString::equals(currentText->getData(),
								XercesString("Permit").c_str())) {
							LOG4CXX_DEBUG(logger, "Decision = Permit");

							std::string obligation = getObligation(response);

							if ((obligation.find(std::string("isolate"))) != std::string::npos) {
								return TNC_IMV_ACTION_RECOMMENDATION_ISOLATE;
							} else {
								return TNC_IMV_ACTION_RECOMMENDATION_ALLOW;
							}
						} else if (XMLString::equals(currentText->getData(),
								XercesString("Deny").c_str())) {
							LOG4CXX_DEBUG(logger, "Decision = Deny");

							std::string obligation = getObligation(response);

							if ((obligation.find(std::string("isolate"))) != std::string::npos) {
								return TNC_IMV_ACTION_RECOMMENDATION_ISOLATE;
							} else {
								return TNC_IMV_ACTION_RECOMMENDATION_NO_ACCESS;
							}
						}
					}
				}
			}
		}
	} catch (xercesc::XMLException& e) {
		char* message = xercesc::XMLString::transcode(e.getMessage());
		LOG4CXX_DEBUG(logger, message);
		XMLString::release(&message);
	}

	return TNC_IMV_ACTION_RECOMMENDATION_NO_RECOMMENDATION;
}

std::string XACMLHelper::getObligation(/*in*/ DOMDocument* response) {
	DOMNodeList* obligations =
			response->getElementsByTagName(
					XercesString("Obligation").c_str());
	const XMLSize_t obligationNodeCount =
			obligations->getLength();
	LOG4CXX_TRACE(logger, "# of obligations: "
			<< obligationNodeCount);

	if (obligationNodeCount > 0) {
		for (XMLSize_t k = 0; k < obligationNodeCount; ++k) {
			DOMNode* currentObligation =
					obligations->item(k);

			if (currentObligation->getNodeType() && // true is not NULL
					currentObligation->getNodeType()
					== DOMNode::ELEMENT_NODE) { // is element node
				// Found node which is an element node. Re-cast node as element
				DOMElement
				* currentObligationElement =
						dynamic_cast<xercesc::DOMElement*> (currentObligation);
				DOMAttr* obligationID = currentObligationElement->getAttributeNode(XercesString("ObligationId").c_str());

				LOG4CXX_DEBUG(logger, "Obligation: " << XercesString(obligationID->getValue()).toString());

				return XercesString(obligationID->getValue()).toString();
			}
		}
	}
	
	return std::string("");
}

DOMDocument* XACMLHelper::createRequest() {
	DOMImplementation* domImplementation = DOMImplementationRegistry::getDOMImplementation(XMLString::transcode("LS"));
	DOMDocument* request = domImplementation->createDocument();
	DOMElement* root = request->createElement(XercesString("Request").c_str());

	/*
	xmlns="urn:oasis:names:tc:xacml:2.0:context:schema:os"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="urn:oasis:names:tc:xacml:2.0:context:schema:os access_control-xacml-2.0-context-schema-os.xsd"
	 */
	root->setAttribute(XercesString("xmlns").c_str(), XercesString("urn:oasis:names:tc:xacml:2.0:context:schema:os").c_str());
	root->setAttribute(XercesString("xmlns:xsi").c_str(), XercesString("http://www.w3.org/2001/XMLSchema-instance").c_str());
	root->setAttribute(XercesString("xsi:schemaLocation").c_str(), XercesString("urn:oasis:names:tc:xacml:2.0:context:schema:os access_control-xacml-2.0-context-schema-os.xsd").c_str());
	request->appendChild(root);

	return request;
}

xercesc::DOMElement* XACMLHelper::createSubject(/*in*/DOMDocument* request) {
	DOMElement* subjectElement = request->createElement(
			XercesString("Subject").c_str());

	return subjectElement;
}

xercesc::DOMElement* XACMLHelper::createSubject(/*in*/DOMDocument* request, /*in*/
std::string subjectCategoy) {
	DOMElement* subjectElement = request->createElement(
			XercesString("Subject").c_str());
	subjectElement->setAttribute(XercesString("SubjectCategory").c_str(),
			XercesString(subjectCategoy).c_str());

	return subjectElement;
}

void XACMLHelper::addResourceActionEnvironmentToRequest(
		/*in*/DOMDocument* request) {
	DOMElement* resourceElement = request->createElement(XercesString(
			"Resource").c_str());
	DOMElement* resourceAttributeElement = XACMLHelper::createAttribute(
			request, XACMLConstants::attribute_id_resource_id,
			XACMLConstants::data_type_xml_string, "network");
	resourceElement->appendChild(resourceAttributeElement);

	DOMElement* actionElement = request->createElement(
			XercesString("Action").c_str());
	DOMElement* actionAttributeElement = XACMLHelper::createAttribute(
				request, XACMLConstants::attribute_id_action_id,
				XACMLConstants::data_type_xml_string, "access");
	actionElement->appendChild(actionAttributeElement);

	DOMElement* environmentElement = request->createElement(XercesString(
			"Environment").c_str());

	request->getDocumentElement()->appendChild(resourceElement);
	request->getDocumentElement()->appendChild(actionElement);
	request->getDocumentElement()->appendChild(environmentElement);
}

void XACMLHelper::addSubjectToRequest(/*in*/xercesc::DOMDocument* request, /*in*/
xercesc::DOMElement* subject) {
	request->getDocumentElement()->appendChild(subject);
}

void XACMLHelper::addAttributeToSubject(/*in*/xercesc::DOMElement* subject, /*in*/
xercesc::DOMElement* attribute) {
	subject->appendChild(attribute);
}

xercesc::DOMElement* XACMLHelper::createAttribute(
/*in*/xercesc::DOMDocument* request, /*in*/std::string attributeID, /*in*/
std::string dataType, /*in*/std::string value) {
	DOMElement* attribute = request->createElement(
			XercesString("Attribute").c_str());
	attribute->setAttribute(XercesString("AttributeId").c_str(), XercesString(
			attributeID).c_str());
	attribute->setAttribute(XercesString("DataType").c_str(), XercesString(
			dataType).c_str());

	DOMElement* attributeValue = request->createElement(XercesString(
			"AttributeValue").c_str());
	DOMText* valueText = request->createTextNode(XercesString(value).c_str());
	attributeValue->appendChild(valueText);
	attribute->appendChild(attributeValue);

	return attribute;
}

xercesc::DOMElement* XACMLHelper::createAttribute(
/*in*/xercesc::DOMDocument* request, /*in*/std::string attributeID, /*in*/
std::string dataType, /*in*/TNC_BufferReference value) {
	DOMElement* attribute = request->createElement(
			XercesString("Attribute").c_str());
	attribute->setAttribute(XercesString("AttributeId").c_str(), XercesString(
			attributeID).c_str());
	attribute->setAttribute(XercesString("DataType").c_str(), XercesString(
			dataType).c_str());

	DOMElement* attributeValue = request->createElement(XercesString(
			"AttributeValue").c_str());
	DOMText* valueText = request->createTextNode(
			XercesString((char*) value).c_str());
	attributeValue->appendChild(valueText);
	attribute->appendChild(attributeValue);

	return attribute;
}

std::string XACMLHelper::writeRequestToString(xercesc::DOMDocument* request) {
	/*
	 Return the first registered implementation that has the desired features. In this case, we are after
	 a DOM implementation that has the LS feature... or Load/Save.
	 */
	DOMImplementation *pImplement = DOMImplementationRegistry::getDOMImplementation(XercesString("LS").c_str());

#if _XERCES_VERSION >= 30000
	DOMLSSerializer* serializer = NULL;
#else
	DOMWriter* serializer = NULL;
#endif

	/*
	 From the DOMImplementation, create a DOMWriter.
	 DOMWriters are used to serialize a DOM tree [back] into an XML document.
	 */
#if _XERCES_VERSION >= 30000
	serializer = ((DOMImplementationLS*) pImplement)->createLSSerializer();
#else
	serializer = ((DOMImplementationLS*) pImplement)->createDOMWriter();
#endif

	/*
	 This line is optional. It just sets a feature of the Serializer to make the output
	 more human-readable by inserting line-feeds, without actually inserting any new elements/nodes
	 into the DOM tree. (There are many different features to set.) Comment it out and see the difference.
	 */
#if _XERCES_VERSION >= 30000
	if (serializer->getDomConfig()->canSetParameter(xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, true)) {
		serializer->getDomConfig()->setParameter(xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, true);
	}
#else
	serializer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
	serializer->setEncoding(XercesString("UTF-8").c_str());
#endif

	// Write the serialized output to the target.
#if _XERCES_VERSION >= 30000
	XercesString result = XercesString(serializer->writeToString(request));
#else
	XercesString result = XercesString(serializer->writeToString(*request));
#endif

	return result.toString();
}

std::string XACMLHelper::exchangeXACMLData(xercesc::DOMDocument* request) {
	tncfhh::Configuration* config = tncfhh::Configuration::instance();
	config->addConfigurationFile(std::string("/etc/tnc/xacml/xacml-pdp.conf"));
	std::string host_ip = config->get(std::string("xacml-pdp-address"));
	std::string host_port = config->get(std::string("xacml-pdp-port"));

	try {
		LOG4CXX_TRACE(logger, "Opening socket.");
		boost::asio::io_service io_service;
		boost::asio::ip::tcp::socket socket(io_service);

		LOG4CXX_TRACE(logger, "Creating resolver.");
		boost::asio::ip::tcp::resolver resolver(io_service);

		LOG4CXX_TRACE(logger, "Querying host.");
		boost::asio::ip::tcp::resolver::query query(host_ip, host_port);

		LOG4CXX_TRACE(logger, "Resolving host.");
		boost::asio::ip::tcp::resolver::iterator it = resolver.resolve(query);

		LOG4CXX_DEBUG(logger, "Connection open.");

		boost::system::error_code error = boost::asio::error::host_not_found;
		socket.connect(*it, error);

		if (error) {
			LOG4CXX_ERROR(logger, "socket.connect error: " << error.message());
//			throw boost::system::system_error(error);
			return std::string("");
		}

		if (socket.is_open()) {
			std::string message = writeRequestToString(request);
			LOG4CXX_DEBUG(logger, "Request: " << message);

			boost::system::error_code ignored_error;
			LOG4CXX_DEBUG(logger, "Sending request ...");
			boost::asio::write(socket, boost::asio::buffer(message),
					boost::asio::transfer_all(), ignored_error);
			LOG4CXX_DEBUG(logger, "Request was send.");

			if (error) {
				LOG4CXX_ERROR(logger, "socket.write error: " << error.message());
				return std::string("");
			}

			LOG4CXX_DEBUG(logger, "Receiving response ...");

			boost::asio::streambuf response;
			std::string responseMessage;
			std::string currentLine;

			for (;;) {
				boost::asio::read_until(socket, response, "</Response>", error);

				// Check that response is OK.
				std::istream responseStream(&response);
				std::getline(responseStream, currentLine);

				responseMessage += currentLine;

				if (error == boost::asio::error::eof) {
					break; // Connection closed cleanly by peer.
				} else if (error) {
					LOG4CXX_ERROR(logger, "socket error: " << error.message());
//					throw boost::system::system_error(error); // Some other error.
					return std::string("");
				}
			}

			LOG4CXX_TRACE(logger, "Shutting down socket.");
			socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
			if (error) {
				LOG4CXX_ERROR(logger, "socket.shutdown error: "
						<< error.message());
			}

			LOG4CXX_TRACE(logger, "Closing socket.");
			socket.close(error);
			if (error) {
				LOG4CXX_ERROR(logger, "socket.close error: " << error.message());
			}
			LOG4CXX_TRACE(logger, "Socket closed.");

			LOG4CXX_DEBUG(logger, "Response: " << responseMessage);

			return responseMessage;
		} else {
			return std::string("");
		}
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}

TNC_IMV_Action_Recommendation XACMLHelper::getXACMLRecommendation(
		/*in*/xercesc::DOMDocument* request) {
	LOG4CXX_DEBUG(logger, "Exchanging XACML data with XACML-PDP");
	std::string responseData = exchangeXACMLData(request);
	LOG4CXX_TRACE(logger, "XACML exchange finished.");

	if (!responseData.empty()) {
		LOG4CXX_TRACE(logger, "Response string received: " << responseData);

		LOG4CXX_DEBUG(logger, "Converting response string to XML document.");
		xercesc::DOMDocument* response = parseIncomingXMLData(responseData);
		LOG4CXX_DEBUG(logger, "Parsing XACML response.");
		TNC_IMV_Action_Recommendation recommendation = parseResponse(response);
		LOG4CXX_DEBUG(logger, "Got recommendation: " << recommendation);

		return recommendation;
	} else {
		LOG4CXX_ERROR(logger, "Response string received was empty");

		return TNC_IMV_ACTION_RECOMMENDATION_NO_RECOMMENDATION;
	}
}

} // namespace iml

} // namespace tncfhh
