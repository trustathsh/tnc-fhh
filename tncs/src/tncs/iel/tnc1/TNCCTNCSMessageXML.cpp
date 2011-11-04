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
 
#include "TNCCTNCSMessageXML.h"

#include <tncutil/XercesString.h>

#include <log4cxx/logger.h>

namespace tncfhh
{

namespace iel
{

namespace tnc1
{

/* for logging */
static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("TNCS.TNC1.TNCCTNCSMessageXML"));

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNCCTNCSMessageXML::TNCCTNCSMessageXML(const TNCCTNCSMessageXML& m) throw (std::runtime_error)
:TNCCTNCSMessage(m)
{
	throw std::runtime_error("Operation copy TNCCTNCSMessageXML not allowed (yet)!");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNCCTNCSMessageXML& TNCCTNCSMessageXML::operator=(const TNCCTNCSMessageXML&) throw (std::runtime_error)
{
	throw std::runtime_error("Operation = to TNCCTNCSMessageXML not allowed (yet)!");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNCCTNCSMessageXML::TNCCTNCSMessageXML(TNC_MessageType type, xercesc::DOMElement * xml) throw (std::runtime_error)
	:TNCCTNCSMessage(type), doc(NULL)
{
	/* check parameter */
	if (!xml)
		throw std::runtime_error("TNCCTNCSMessageXML::TNCCTNCSMessageXML: invalid parameter: null pointer");

	/* get THE XerxesC implementation */
	xercesc::DOMImplementation * impl = xercesc::DOMImplementationRegistry::getDOMImplementation(XercesString("LS").c_str());
	if (!impl)
		throw std::runtime_error("TNCCTNCSMessageXML::constructor: Xerces error: No DOMImplementation LS found!");

	// cleate a new XML-document
	this->doc = impl->createDocument();

	// (deep) copy the XML node and add as root
	this->doc->appendChild(this->doc->importNode(xml, true));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNCCTNCSMessageXML::TNCCTNCSMessageXML(TNC_MessageType type, xercesc::DOMDocument * doc) throw (std::runtime_error)
	:TNCCTNCSMessage(type), doc(doc)
{
	/* check parameter */
	if (!doc)
		throw std::runtime_error("TNCCTNCSMessageXML::TNCCTNCSMessageXML: invalid parameter: null pointer");
	else if (!doc->getDocumentElement()) {
		doc->release();
		throw std::runtime_error("TNCCTNCSMessageXML::TNCCTNCSMessageXML: invalid parameter (root): null pointer");
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNCCTNCSMessageXML::~TNCCTNCSMessageXML()
{
	if (this->doc)
		this->doc->release();
	this->doc = NULL;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
xercesc::DOMElement * TNCCTNCSMessageXML::getXML()
{
	return this->doc->getDocumentElement();
}

} // namespace tnc1

} // namespace iel

} // namespace tncfhh
