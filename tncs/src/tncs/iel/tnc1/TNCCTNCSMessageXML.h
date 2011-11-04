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
#ifndef TNCS_TNCCTNCSMESSAGEXML_H_
#define TNCS_TNCCTNCSMESSAGEXML_H_

#include "TNCCTNCSMessage.h"
#include <stdexcept>
//#include "../../exception/TNCCSProtocolNotSupportedException.h"

#include <xercesc/dom/DOM.hpp>

namespace tncfhh {

namespace iel {

namespace tnc1 {

/**
 * This Class represent an TNCC-TNCS-Message with XML data. This message
 * is specified in TNC IF-TNCCS.
 * 
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>05.03.2008 - create class (mbs)</li>
 *   <li>20.07.2009 - redesign for 0.6.0 (mbs)</li>
 *   <li>28.08.2009 - redesign the small classes (remove xml)
 *   	and write doc (mbs)</li>
 * </ul>
 *
 * @class TNCCTNCSMessageXML
 * @brief Represent a TNCC-TNCS-Message with XML data
 * @date 05.03.2008
 * @author Mike Steinmetz (mbs)
 * @see TNCCTNCSMessage
 * @see TNCCTNCSMessageBase64
 */
class TNCCTNCSMessageXML : public TNCCTNCSMessage
{
public:
	/**
	 * Create an TNCCTNCSMessageXML based on an XML element.
	 *
	 * @param type The MessageType (see spec).
	 * @param xml Create an deep copy from this XML document.
	 */
	TNCCTNCSMessageXML(TNC_MessageType type, xercesc::DOMElement * xml) throw (std::runtime_error);

	/**
	 * Create an TNCCTNCSMessageXML based on a DOM document. The root element
	 * of this document is append below XML tag in TNCC-TNCS-Message (see spec).
	 *
	 * <b>NOTE:</b> Please do not release the DOMDocument, because this class
	 * release the parameter <code>doc</code>.
	 *
	 * @param type The MessageType (see spec).
	 * @param doc DOMDucument with root element (this released by this class).
	 */
	TNCCTNCSMessageXML(TNC_MessageType type, xercesc::DOMDocument * doc) throw (std::runtime_error);
	
	/**
	 *
	 */
	virtual ~TNCCTNCSMessageXML();

	/**
	 * Get the DOM document below the XML tag in TNCC-TNCS-Message (see
	 * IF-TNCCS specification).
	 *
	 * @return The XML content. Return <code>NULL</code> if no XML content found.
	 *
	 */
	virtual xercesc::DOMElement * getXML();

private:
	/**
	 * This Constructor is not allowed.
	 */
	TNCCTNCSMessageXML(const TNCCTNCSMessageXML&) throw (std::runtime_error);

	/**
	 * This Function is not allowed.
	 */
	TNCCTNCSMessageXML& operator=(const TNCCTNCSMessageXML&) throw (std::runtime_error);

	/**
	 * The document that contain the 'any' XML tag as root element.
	 */
	xercesc::DOMDocument * doc;
};

}  // namespace tnc1

}  // namespace iel

}  // namespace tncfhh

#endif /*TNCCTNCSMESSAGEXML_H_*/
