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
#ifndef TNCS_IELTNCCSBATCH_H_
#define TNCS_IELTNCCSBATCH_H_

#include "../TNCCSData.h"
#include "IMCIMVMessage.h"
#include "TNCCTNCSMessage.h"
#include <tncutil/XercesString.h>
#include "../../exception/TNCCSProtocolNotSupportedException.h"

#include <list>
#include <exception>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOM.hpp>

namespace tncfhh {

namespace iel {

namespace tnc1 {

/**
 * TODO_Comment
 * 
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>25.02.2008 - create class (mbs)</li>
 *   <li>27.02.2008 - add first uncomplete XML parsing (mbs)</li>
 *   <li>28.02.2008 - add first uncomplete XML creating (mbs)</li>
 *   <li>22.04.2008 - add resetID [bug 9] (mbs)</li>
 *   <li>15.04.2008 - remove an XML (xsi:schemalocation) bug [bug] (mbs)</li>
 *   <li>20.07.2009 - redesign for 0.6.0 (mbs)</li>
 *   <li>28.08.2009 - replace old C code (for convert string to int) to C++ code (mbs)</li>
 *   <li>02.09.2009 - make XercesC 3.0 compatible [bug 24] (mbs)</li>
 *   <li>02.09.2009 - make XercesC 3.0.1 compatible (ib)</li>
 * </ul>
 *
 * @class TNCCSBatch
 * @brief TODO_Brief
 * @date 25.02.2008
 * @author Mike Steinmetz (mbs)
 */
class TNCCSBatch
{
public:
	enum Recipient {TNCS, TNCC};
	TNCCSBatch(Recipient recipient);
	virtual ~TNCCSBatch();
	virtual TNCCSBatch & operator=(const TNCCSData & tnccsData) throw (TNCCSProtocolNotSupportedException);

	/**
	 * TODO_Comment
	 *
	 * @param tnccTncsMessage the TNCC-TNCS-Message. NOTE: this class delete the tnccTncsMessage instance
	 */
	virtual void addTNCCTNCSMessage(TNCCTNCSMessage * tnccTncsMessage);

	/**
	 * TODO_Comment
	 *
	 * @param imcImvMessage the IMC-IMV-Message. NOTE: this class delete the imcImvMessage instance
	 */
	virtual void addIMCIMVMessage(IMCIMVMessage * imcImvMessage);
	
	virtual void deleteAllMessages();
	virtual void resetID();
	
	virtual TNCCSData getNewTnccsData() throw (std::runtime_error);
	
	virtual std::list<TNCCTNCSMessage *> getTnccTncsMessages();
	virtual std::list<IMCIMVMessage *> getImcImvMessages();

	static const XercesString iftnccsNamespaceURI;
private:
	explicit TNCCSBatch(const TNCCSBatch &batch) throw (std::runtime_error);

	virtual void addTnccTncsMessage(xercesc::DOMElement & tnccTncsMessage) throw (TNCCSProtocolNotSupportedException);
	virtual void addImcImvMessage(xercesc::DOMElement & imcImvMessage) throw (TNCCSProtocolNotSupportedException);
	virtual TNC_UInt32 getType(xercesc::DOMElement & type) throw (TNCCSProtocolNotSupportedException);
	
	TNC_UInt32 id;
	Recipient recipient;
	std::list<TNCCTNCSMessage *> tnccTncsMessages;
	std::list<IMCIMVMessage *> imcImvMessages;
	
	class XercesInit {
	public:
		XercesInit();
		~XercesInit();
	};
	static XercesInit xercesInit;

	/**
	 * Retrieves the value of an attribute of the specified node.
	 *
	 * @param node The node to search for the attribute.
	 * @param attName The name of the searched attribute.
	 * @throw An exception if the attribute was not found in the specified node.
	 *
	 */
	XercesString getAttributeValue(xercesc::DOMNode & node, const XercesString & attName) throw(TNCCSProtocolNotSupportedException);

	/**
	 * Writes at most 'size' bytes from 'format' to 'buf'. 'format' is a
	 * format-string as used by ordinary printf functions.
	 *
	 * @param buf The buffer where to write the array.
	 * @param size Size of the buffer.
	 * @param format A format string.
	 */
    void writeToArray(char *buf, const int size, const char *format, ...);
};

}  // namespace tnc1

}  // namespace iel

}  // namespace tncfhh

#endif /*TNCS_IELTNCCSBATCH_H_*/
