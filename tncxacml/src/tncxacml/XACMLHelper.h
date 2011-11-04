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
 
#ifndef XACMLHELPER_H_
#define XACMLHELPER_H_

#include <tcg/tnc/tncifimv.h>
#include <tncutil/XercesString.h>

// Mandatory for using any feature of Xerces.
#include <xercesc/util/PlatformUtils.hpp>

// Use the Document Object Model (DOM) API
#include <xercesc/dom/DOM.hpp>

// Required for outputing a Xerces DOMDocument to a standard output stream (Also see: XMLFormatTarget)
#include <xercesc/framework/StdOutFormatTarget.hpp>

// Required for outputing a Xerces DOMDocument to the file system (Also see: XMLFormatTarget)
#include <xercesc/framework/LocalFileFormatTarget.hpp>

#include <xercesc/parsers/XercesDOMParser.hpp>

// The following includes are only used to create sample data in this tutorial. If you don't need the sample data, then you don't need these headers.
#include <string>
#include <sstream>

/**
 * Handling of the XACML-network traffic
 */
#include <iostream>
#include <boost/asio.hpp>

XERCES_CPP_NAMESPACE_USE

namespace tncfhh {

namespace iml {

/**
 * Class that .
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>17.01.2011 - first version (bhl)</li>
 * </ul>
 *
 * @date 17.01.2011
 * @author Bastian Hellmann (bhl)
 */
class XACMLHelper {
public:
	static xercesc::DOMDocument* createRequest();

	static xercesc::DOMElement* createSubject(/*in*/ xercesc::DOMDocument* request);

	static xercesc::DOMElement* createSubject(/*in*/ xercesc::DOMDocument* request, /*in*/ std::string subjectCategory);

	static void addResourceActionEnvironmentToRequest(/*in*/ xercesc::DOMDocument* request);

	static xercesc::DOMElement* createAttribute(/*in*/ xercesc::DOMDocument* request, /*in*/ std::string attributeID, /*in*/ std::string dataType, /*in*/ TNC_BufferReference value);

	static xercesc::DOMElement* createAttribute(/*in*/ xercesc::DOMDocument* request, /*in*/ std::string attributeID, /*in*/ std::string dataType, /*in*/ std::string value);

	static void addSubjectToRequest(/*in*/ xercesc::DOMDocument* request, /*in*/ xercesc::DOMElement* subject);

	static void addAttributeToSubject(/*in*/ xercesc::DOMElement* subject, /*in*/ xercesc::DOMElement* attribute);

	static TNC_IMV_Action_Recommendation getXACMLRecommendation(/*in*/ xercesc::DOMDocument* request);

	static std::string writeRequestToString(/*in*/ xercesc::DOMDocument* request);

	XACMLHelper();
	virtual ~XACMLHelper();

private:
	static std::string exchangeXACMLData(/*in*/ xercesc::DOMDocument* request);

	static xercesc::DOMDocument* parseIncomingXMLData(/*in*/ std::string incomingString);

	static TNC_IMV_Action_Recommendation parseResponse(/*in*/ xercesc::DOMDocument* response);

	static std::string getObligation(/*in*/ DOMDocument* response);

	class XercesInit {
		public:
			XercesInit();
			~XercesInit();
	};

	static XercesInit xercesInit;
};

} // namespace iml

} // namespace tncfhh

#endif /* XACMLHELPER_H_ */
