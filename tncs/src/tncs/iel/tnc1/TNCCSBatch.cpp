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
#include "TNCCSBatch.h"
#include "TNCCTNCSMessageXML.h"
#include "TNCCTNCSMessageBase64.h"
#include "../string_helper.h"

#include <cstdio>
#include <stdlib.h>
#include <errno.h>
//#include <TNCUtil/io/TNCIOHelper.h>
#include <tcg/tnc/iftnccsschema.h>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/Base64.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/MemBufFormatTarget.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/validators/common/Grammar.hpp>
#include <sstream>
#include <log4cxx/logger.h>
#include <stdarg.h>
#include <iomanip>
#include <xercesc/util/XercesVersion.hpp>
#include <xercesc/util/XMLString.hpp>


namespace tncfhh {

namespace iel {

namespace tnc1 {

/* for logging */
static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("TNCS.TNC1.TNCCSBatch"));

TNCCSBatch::XercesInit TNCCSBatch::xercesInit;
const XercesString TNCCSBatch::iftnccsNamespaceURI("http://www.trustedcomputinggroup.org/IWG/TNC/1_0/IF_TNCCS#");

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNCCSBatch::XercesInit::XercesInit() {

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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNCCSBatch::XercesInit::~XercesInit() {

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

/**
 * This is a StrictErrorHandler for parsing the XML document.
 * 
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>26.02.2008 - create class (mbs)</li>
 * </ul>
 *
 * @class StrictErrorHandler
 * @brief StrictErrorHandler for parsing.
 * @date 26.02.2008
 * @author Mike Steinmetz (mbs)
 */
class StrictErrorHandler : public xercesc::DefaultHandler
{
	virtual void error (const xercesc::SAXParseException &exc)
	{
		throw exc;
	}

	virtual void fatalError (const xercesc::SAXParseException &exc)
	{
		throw exc;
	}

	virtual void warning (const xercesc::SAXParseException &exc)
	{
		throw exc;
	}
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNCCSBatch::TNCCSBatch(Recipient recipient)
:id(0), recipient(recipient)
{
	// SSL = Super Sexy Lady
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNCCSBatch::TNCCSBatch(const TNCCSBatch &batch) throw (std::runtime_error)
{
	throw std::runtime_error("TNCCSBatch Copy Constructor not allowed.");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNCCSBatch::~TNCCSBatch()
{
	this->deleteAllMessages();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNCCSBatch & TNCCSBatch::operator=(const TNCCSData & tnccsData) throw (TNCCSProtocolNotSupportedException)
{
	deleteAllMessages();

	/* parse XML */
	LOG4CXX_TRACE(logger, "Parse XML TNCCS")

	/* get if-tnccs-schema */
	std::string schema(TNC_IF_TNCCS_SCHEMA);
	
	/* create new xerces parser */
	xercesc::XercesDOMParser * parser = new xercesc::XercesDOMParser();

	/* create new strict error handler */
	xercesc::ErrorHandler * errorHandler = new StrictErrorHandler();

	/* set strict error handler */
	parser->setErrorHandler(errorHandler);
	
	try {
		/* set schema */
		xercesc::MemBufInputSource inputSourceSchema((XMLByte*)schema.c_str(), schema.length(), "Schema");
		if (!parser->loadGrammar(inputSourceSchema, xercesc::Grammar::SchemaGrammarType, true))
			throw TNCCSProtocolNotSupportedException("Grammar is NULL");
		/* set parser parameter */
		parser->setValidationScheme(xercesc::XercesDOMParser::Val_Always);
		parser->setDoSchema(true);
		parser->setDoNamespaces(true);
		parser->useCachedGrammarInParse(true);
		parser->setValidationSchemaFullChecking(true);
		parser->setValidationConstraintFatal(true);
		
		/* parse document */
		xercesc::MemBufInputSource inputSource(tnccsData.getData(), tnccsData.getLength(), "XML");
		parser->parse(inputSource);
		xercesc::DOMDocument * doc = parser->getDocument();
		
		/* get root element */
		xercesc::DOMElement * xmlBatch = doc->getDocumentElement();
		
		/* read of BatchId */
		std::stringstream idStringStream(getAttributeValue(*xmlBatch, "BatchId").toString());
		TNC_UInt32 id = 0;
		idStringStream >> id;

		/* check of BatchId */
		if (idStringStream.fail())
			throw TNCCSProtocolNotSupportedException("tnc1::TNCCSBatch: Error by reading XML attribute 'BatchId'.");
		if (this->id==0 && id==1)
			this->id = 1;
		else if (this->id==0 && id!=1)
			throw TNCCSProtocolNotSupportedException(std::string("tnc1::TNCCSBatch: BatchId 1 expected but get BatchId ") + id);
		else if (id != this->id+2)
			throw TNCCSProtocolNotSupportedException(std::string("tnc1::TNCCSBatch: BatchId ") + (this->id+2) + " expected but get BatchId " + id);
		else
			this->id += 2;

		
		/* check recipient */
		switch (this->recipient) {
			case TNCC:
				if (getAttributeValue(*xmlBatch, "Recipient").compare(XercesString("TNCC")))
					throw TNCCSProtocolNotSupportedException("tnc1::TNCCSBatch: Recipient is not TNCC");
				break;
			case TNCS:
				if (getAttributeValue(*xmlBatch, "Recipient").compare(XercesString("TNCS")))
					throw TNCCSProtocolNotSupportedException("tnc1::TNCCSBatch: Recipient is not TNCS");
				break;
			default:
				throw TNCCSProtocolNotSupportedException("tnc1::TNCCSBatch: Recipient is not defined");
		}
		
		
		/* read all TNCC-TNCS-Messages */
		xercesc::DOMNode * xmlNode = xmlBatch->getFirstChild();
		while (xmlNode) {
			xercesc::DOMElement * xmlElement = dynamic_cast<xercesc::DOMElement*>(xmlNode);
			/* check if TNCC-TNCS-Message */
			if (xmlElement && !XercesString(xmlElement->getTagName()).compare(XercesString("TNCC-TNCS-Message"))){
				/* add TNCC-TNCS-Message */
				addTnccTncsMessage(*xmlElement);
			}else if(xmlElement)
				/* goto next (no TNCC-TNCS-Message follow) */
				break;

			xmlNode = xmlNode->getNextSibling();
		}
		
		LOG4CXX_DEBUG(logger, "PROCESS IMCV MESSAGES");
		
		/* read all IMC-IMV-Messages */
		while (xmlNode) {
			xercesc::DOMElement * xmlElement = dynamic_cast<xercesc::DOMElement*>(xmlNode);
			/* check if IMC-IMV-Message */
			if (xmlElement && !XercesString(xmlElement->getTagName()).compare(XercesString("IMC-IMV-Message"))){
				/* add IMC-IMV-Message */
				addImcImvMessage(*xmlElement);
			}else if(xmlElement)
				/* goto next (no IMC-IMV-Message follow) */
				break;

			xmlNode = xmlNode->getNextSibling();
		}
		
	} catch (const xercesc::OutOfMemoryException &) {
		delete errorHandler;
		delete parser;
		throw TNCCSProtocolNotSupportedException("tnc1::TNCCSBatch::operator=: OutOfMemoryException");
	} catch (const xercesc::XMLException & e) {
		delete errorHandler;
		delete parser;
		throw TNCCSProtocolNotSupportedException("tnc1::TNCCSBatch::operator=: An error occurred during parsing: " + XercesString(e.getMessage()).toString());
	} catch (const xercesc::DOMException & e) {
		delete errorHandler;
		delete parser;
		throw TNCCSProtocolNotSupportedException(std::string("tnc1::TNCCSBatch::operator=: An error occurred during parsing: DOM Error during parsing. DOMException code is: ") + e.code);
	} catch (const xercesc::SAXParseException & e) {
		delete errorHandler;
		delete parser;
		throw TNCCSProtocolNotSupportedException(std::string("tnc1::TNCCSBatch::operator=: ") +
				XercesString(e.getSystemId()).toString() +
				" (" + e.getLineNumber() + ":" +
				e.getColumnNumber() + ") " +
				XercesString(e.getMessage()).toString()
		);
	} catch (TNCCSProtocolNotSupportedException & e) {
		delete errorHandler;
		delete parser;
		throw e;
	} catch (...) {
		delete errorHandler;
		delete parser;
		throw TNCCSProtocolNotSupportedException("tnc1::TNCCSBatch::operator=: An unknown error occurred during parsing.");
	}
	
	delete errorHandler;
	delete parser;

	return *this;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TNCCSBatch::deleteAllMessages()
{
	/* iterate all about TNCC-TNCS-Messages and delete the messages */
	for(std::list<TNCCTNCSMessage*>::const_iterator iter = this->tnccTncsMessages.begin();
			iter!=this->tnccTncsMessages.end();
			iter++)
	{
		delete *iter;
	}
	this->tnccTncsMessages.clear();

	/* iterate all about IMC-IMV-Messages and delete the messages */
	for(std::list<IMCIMVMessage*>::const_iterator iter = this->imcImvMessages.begin();
			iter!=this->imcImvMessages.end();
			iter++)
	{
		delete *iter;
	}
	this->imcImvMessages.clear();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TNCCSBatch::resetID()
{
	this->id = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TNCCSBatch::addTnccTncsMessage(xercesc::DOMElement & tnccTncsMessage) throw (TNCCSProtocolNotSupportedException)
{

	/* get Base64 tag */
	xercesc::DOMElement * xmlBase64 = dynamic_cast<xercesc::DOMElement*>(tnccTncsMessage.getElementsByTagName(XercesString("Base64").c_str())->item(0));
	
	if (xmlBase64) {
		/* found Base64 */
		
		/* get text element (the content of this element is Base64) */
		xercesc::DOMText * xmlText = dynamic_cast<xercesc::DOMText*>(xmlBase64->getFirstChild());
		
		/* declare binary */
#if _XERCES_VERSION < 30001
		unsigned int binLength = 0;
#else
		XMLSize_t binLength = 0;
#endif
		TNC_BufferReference bin = NULL;

		/* check text element is exist */
		if (xmlText) {

			/* get text (base64) */
			const XMLCh * base64 = xmlText->getNodeValue();

			/* check text (base64) is exist */
			if (!base64)
				throw TNCCSProtocolNotSupportedException("tnc1::addTnccTncsMessage: Base64 problem. base64 is NULL!?");

			/* decode text (base64) into binary bytes */
			bin = xercesc::Base64::decodeToXMLByte(base64, &binLength);
		}

		/* get MessageType */
		TNC_MessageType type = getType(*dynamic_cast<xercesc::DOMElement*>(tnccTncsMessage.getElementsByTagName(XercesString("Type").c_str())->item(0)));
		
		/* add TNCC-TNCS-Message */
		this->tnccTncsMessages.push_back(new TNCCTNCSMessageBase64(type, bin, binLength));
		
		/* delete dynamically Xerces buffer */
		xercesc::XMLString::release((char**)&bin);

		return;
	}
	
	/* else */
	
	/* found any XML */

	/* get XML tag */
	xercesc::DOMElement * xmlXml = dynamic_cast<xercesc::DOMElement*>(tnccTncsMessage.getElementsByTagName(XercesString("XML").c_str())->item(0));
	
	/* check XML */
	if (!xmlXml)
			throw TNCCSProtocolNotSupportedException("tnc1::addTnccTncsMessage: Cast problem, unknown type!");

	/* get the first any XML (in XML tag). ignore all non DOMElements */
	xercesc::DOMNode * xmlAny = xmlXml->getFirstChild();
	while(xmlAny && !dynamic_cast<xercesc::DOMElement*>(xmlAny)){
		xmlAny = xmlAny->getNextSibling();
	}
	
	/* check any XML */
	if (!xmlAny)
			throw TNCCSProtocolNotSupportedException("tnc1::addTnccTncsMessage: No any XML element found!");
	
	/* get MessageType */
	TNC_MessageType type = getType(*dynamic_cast<xercesc::DOMElement*>(tnccTncsMessage.getElementsByTagName(XercesString("Type").c_str())->item(0)));
	
	try {
		/* add TNCC-TNCS-Message */
		this->tnccTncsMessages.push_back(new TNCCTNCSMessageXML(type, dynamic_cast<xercesc::DOMElement*>(xmlAny)));
	} catch (std::exception& e) {
		throw TNCCSProtocolNotSupportedException(e.what());
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TNCCSBatch::addImcImvMessage(xercesc::DOMElement & imcImvMessage) throw (TNCCSProtocolNotSupportedException)
{
	
	/* get Base64 tag */
	xercesc::DOMElement * xmlBase64 = dynamic_cast<xercesc::DOMElement*>(imcImvMessage.getElementsByTagName(XercesString("Base64").c_str())->item(0));

	/* get text element */
	xercesc::DOMText * xmlText = dynamic_cast<xercesc::DOMText*>(xmlBase64->getFirstChild());
	
	/* declare binary */
#if _XERCES_VERSION < 30001
	unsigned int binLength = 0;
#else
	XMLSize_t binLength = 0;
#endif

	TNC_BufferReference bin = NULL;

	/* check text element */
	if (xmlText) {
		/* get text (base64) */
		const XMLCh * base64 = xmlText->getNodeValue();

		/* check text (base64) */
		if (!base64)
			throw TNCCSProtocolNotSupportedException("tnc1::addImcImvMessage: Base64 problem. base64 is NULL!?");

		/* decode text (base64) into binary bytes */
		bin = xercesc::Base64::decodeToXMLByte(base64, &binLength, 0, xercesc::Base64::Conf_Schema);
	}

	/* get MessageType */
	TNC_MessageType type = getType(*dynamic_cast<xercesc::DOMElement*>(imcImvMessage.getElementsByTagName(XercesString("Type").c_str())->item(0)));
	
	/* check type is not 0xffffffxx or 0xxxxxxxff */
	if ((type & 0xffffff00) == 0xffffff00 || (type & 0xff) == 0xff) {
		std::stringstream s;
		s<<std::setw(8) << std::setfill('0') << std::hex<<type;
		throw TNCCSProtocolNotSupportedException(std::string("Wrong MessageType 0x") + s.str() + " in XML.");
	}

	/* add IMC-IMV-Message */
	this->imcImvMessages.push_back(new IMCIMVMessage(bin, binLength, type));

	/* delete dynamically Xerces buffer */
	xercesc::XMLString::release((char**)&bin);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_UInt32 TNCCSBatch::getType(xercesc::DOMElement & xmlType) throw (TNCCSProtocolNotSupportedException)
{
	/* get text element */
	xercesc::DOMText * xmlText = dynamic_cast<xercesc::DOMText*>(xmlType.getFirstChild());
	
	/* check text element */
	if (!xmlText)
		throw TNCCSProtocolNotSupportedException("tnc1::getType: MessageType problem");
	
	/* get text (messageType) */
	const XMLCh * messageTypeText = xmlText->getNodeValue();
	
	/* check text (messageType) */
	if (!messageTypeText)
		throw TNCCSProtocolNotSupportedException("tnc1::getType: MessageType problem. messageTypeText is NULL!?");
	
	/* read of MessageType */
	std::stringstream typeStringStream(XercesString(messageTypeText).toString());
	TNC_MessageType type = 0;
	typeStringStream >> std::hex >> type;

	/* check of MessageType */
	if (typeStringStream.fail())
		throw TNCCSProtocolNotSupportedException("tnc1::TNCCSBatch: Error by hex-string to TNC_MessageType.");

	return type;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TNCCSBatch::addTNCCTNCSMessage(TNCCTNCSMessage * tnccTncsMessage)
{
	this->tnccTncsMessages.push_back(tnccTncsMessage);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TNCCSBatch::addIMCIMVMessage(IMCIMVMessage * imcImvMessage)
{
	this->imcImvMessages.push_back(imcImvMessage);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::list<TNCCTNCSMessage *> TNCCSBatch::getTnccTncsMessages()
{
	return this->tnccTncsMessages;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::list<IMCIMVMessage *> TNCCSBatch::getImcImvMessages()
{
	return this->imcImvMessages;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNCCSData TNCCSBatch::getNewTnccsData() throw (std::runtime_error)
{
	xercesc::DOMImplementation * impl = xercesc::DOMImplementationRegistry::getDOMImplementation(XercesString("LS").c_str());
	if (!impl)
		throw std::runtime_error("TNCCSBatch::getNewNalTnccsBatch: Xerces error: No DOMImplementation LS found!");
	xercesc::DOMDocument * doc = impl->createDocument(iftnccsNamespaceURI.c_str(), XercesString("TNCCS-Batch").c_str(), NULL);

	try {
		
		xercesc::DOMElement *root = doc->getDocumentElement();
	
		/* set BatchID */
		this->id += 2; // increment BatchID
		root->setAttribute(XercesString("BatchId").c_str(), XercesString(std::string("") + this->id).c_str());
		switch (this->recipient) {
			case TNCC:
				root->setAttribute(XercesString("Recipient").c_str(), XercesString("TNCC").c_str());
				break;
			case TNCS:
				root->setAttribute(XercesString("Recipient").c_str(), XercesString("TNCS").c_str());
				break;
		}
		root->setAttribute(XercesString("xsi:schemalocation").c_str(), XercesString("http://www.trustedcomputinggroup.org/IWG/TNC/1_0/IF_TNCCS# https://www.trustedcomputinggroup.org/XML/SCHEMA/TNCCS_1.0.xsd").c_str());
		root->setAttribute(XercesString("xmlns:xsi").c_str(), XercesString("http://www.w3.org/2001/XMLSchema-instance").c_str());
		
		/* iterate over all TNCC-TNCS-Messages */
		for(std::list<TNCCTNCSMessage*>::const_iterator iter = this->tnccTncsMessages.begin();
				iter!=this->tnccTncsMessages.end();
				iter++)
		{
			xercesc::DOMElement *xmlType = doc->createElementNS(iftnccsNamespaceURI.c_str(), XercesString("Type").c_str());
			char buff[9];
			writeToArray(buff,9,"%08X",(*iter)->getType());
			xercesc::DOMText *xmlTypeContent = doc->createTextNode(XercesString((char*)buff).c_str());
			xmlType->appendChild(xmlTypeContent);

			xercesc::DOMElement * xmlBase64OrXml = NULL;
			if (dynamic_cast<TNCCTNCSMessageBase64*>(*iter)) {
				TNCCTNCSMessageBase64 * ttmb = dynamic_cast<TNCCTNCSMessageBase64*>(*iter);

				xmlBase64OrXml = doc->createElementNS(iftnccsNamespaceURI.c_str(), XercesString("Base64").c_str());
				/* convert msg in Base64 */
#if _XERCES_VERSION < 30001
				unsigned int msgAsBase64Length = 0;
#else
				XMLSize_t msgAsBase64Length = 0;
#endif
				XMLByte * msgAsBase64 = xercesc::Base64::encode(ttmb->getData(), ttmb->getDataLength(), &msgAsBase64Length);
				/* add Base64 to XML */
				xmlTypeContent = doc->createTextNode(XercesString((char*)msgAsBase64).c_str());
				/* delete Base64 */
				xercesc::XMLString::release((char**)&msgAsBase64);
				xmlBase64OrXml->appendChild(xmlTypeContent);
			} else if (dynamic_cast<TNCCTNCSMessageXML*>(*iter)) {
				TNCCTNCSMessageXML * ttmx = dynamic_cast<TNCCTNCSMessageXML*>(*iter);
				xmlBase64OrXml = doc->createElementNS(iftnccsNamespaceURI.c_str(), XercesString("XML").c_str());
				/* get any XML */
				xercesc::DOMNode * anyXml = doc->importNode(ttmx->getXML(), true);

				xmlBase64OrXml->appendChild(anyXml);
			} else
				throw std::runtime_error("TNCCSBatch::getNewNalTnccsBatch: unknown TNCC-TNCS-Message");

			xercesc::DOMElement * xmlImcImvMessage = doc->createElementNS(iftnccsNamespaceURI.c_str(), XercesString("TNCC-TNCS-Message").c_str());
			xmlImcImvMessage->appendChild(xmlType);
			xmlImcImvMessage->appendChild(xmlBase64OrXml);

			root->appendChild(xmlImcImvMessage);
		}

		
		/* iterate over all IMC-IMV-Messages */
		for(std::list<IMCIMVMessage*>::const_iterator iter = this->imcImvMessages.begin();
				iter!=this->imcImvMessages.end();
				iter++)
		{
			xercesc::DOMElement * xmlType = doc->createElementNS(iftnccsNamespaceURI.c_str(), XercesString("Type").c_str());
			char buff[9];
			writeToArray(buff,9,"%08X",(*iter)->getType());
			xercesc::DOMText *xmlTypeContent = doc->createTextNode(XercesString((char*)buff).c_str());
			xmlType->appendChild(xmlTypeContent);


			xercesc::DOMElement *xmlBase64 = doc->createElementNS(iftnccsNamespaceURI.c_str(), XercesString("Base64").c_str());
			/* convert msg in Base64 */
#if _XERCES_VERSION < 30001
			unsigned int msgAsBase64Length = 0;
#else
			XMLSize_t msgAsBase64Length = 0;
#endif
			XMLByte * msgAsBase64 = xercesc::Base64::encode((*iter)->getData(), (*iter)->getDataLength(), &msgAsBase64Length);
			if (msgAsBase64) {
				/* add Base64 to XML */
				xercesc::DOMText *xmlBase64Content = doc->createTextNode(XercesString((char*)msgAsBase64).c_str());
				xmlBase64->appendChild(xmlBase64Content);
			}
			/* delete Base64 */
			xercesc::XMLString::release((char**)&msgAsBase64);


			xercesc::DOMElement * xmlImcImvMessage = doc->createElementNS(iftnccsNamespaceURI.c_str(), XercesString("IMC-IMV-Message").c_str());
			xmlImcImvMessage->appendChild(xmlType);
			xmlImcImvMessage->appendChild(xmlBase64);


			root->appendChild(xmlImcImvMessage);
		}

		xercesc::MemBufFormatTarget target;
#if _XERCES_VERSION >= 30000
		xercesc::DOMLSSerializer * serializer = impl->createLSSerializer();
		if (serializer->getDomConfig()->canSetParameter( xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, true))
			serializer->getDomConfig()->setParameter( xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, true);

		xercesc::DOMLSOutput * output =  impl->createLSOutput();
		output->setByteStream(&target);
		if (!serializer->write(doc, output)) {
			output->release();
			serializer->release();
			throw std::runtime_error("TNCCSBatch::getNewNalTnccsBatch: error on serialize");
		}
		output->release();
#else
		xercesc::DOMWriter * serializer = impl->createDOMWriter();
		serializer->setFeature(XercesString("format-pretty-print").c_str(), true);
		serializer->setEncoding(XercesString("UTF-8").c_str());
		if (!serializer->writeNode(&target, *doc)) {
			serializer->release();
			throw std::runtime_error("TNCCSBatch::getNewNalTnccsBatch: error on serialize");
		}
#endif
	
		try {
			TNCCSData ret((TNC_BufferReference)target.getRawBuffer(), target.getLen());

			/* released memory */
			serializer->release();
			doc->release();

			return ret;
		} catch (...) {
			serializer->release();
			throw;
		}
	} catch (...) {
		/* released memory */
		doc->release();
		throw;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
XercesString TNCCSBatch::getAttributeValue(xercesc::DOMNode & node, const XercesString & attName) throw(TNCCSProtocolNotSupportedException)
{
    xercesc::DOMNamedNodeMap * attributeList = node.getAttributes();
    if (attributeList == NULL)
    	throw TNCCSProtocolNotSupportedException("xerces attributeList == NULL");

    xercesc::DOMNode * attribute = attributeList->getNamedItem(attName.c_str());
    if (attribute == NULL)
    	throw TNCCSProtocolNotSupportedException("xerces attribute not exist");

   	return attribute->getNodeValue();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TNCCSBatch::writeToArray(char *buf, const int size, const char *format, ...)
{
    va_list l;
    va_start(l, format);
    vsnprintf(buf, size, format, l);
}


} // namespace tnc1

} // namespace iel

} // namespace NAATNCS
