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
#include "XercesString.h"
#include <xercesc/util/XMLString.hpp>

namespace tncfhh
{

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *                  XercesString::XercesString(const char*)                  *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

XercesString::XercesString(const char * str)
{
//	std::cout << "XercesString::ctor" << std::endl;
	XMLCh * xstr = xercesc::XMLString::transcode(str);
	this->operator=(xstr);
	xercesc::XMLString::release(&xstr);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *                XercesString::XercesString(const std::string&)             *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
XercesString::XercesString(const std::string & str)
{
	XMLCh * xstr = xercesc::XMLString::transcode(str.c_str());
	this->operator=(xstr);
	xercesc::XMLString::release(&xstr);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *                      XercesString::XercesString(const XMLCh*)             *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
XercesString::XercesString(const XMLCh * xstr)
	:std::basic_string<XMLCh>(xstr)
{
//	std::cout << "XercesString::c-ctor" << std::endl;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *                        XercesString::~XercesString()                      *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
XercesString::~XercesString()
{
//	std::cout << "XercesString::dtor" << std::endl;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *         XercesString & XercesString::operator=(const XMLCh * xstr)        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
XercesString & XercesString::operator=(const XMLCh * xstr)
{
	std::basic_string<XMLCh>::operator=(xstr);
	return *this;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *           std::string& XercesString::copyToString(std::string&)           *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::string & XercesString::copyToString(std::string & sstr)
{
	const char * str = xercesc::XMLString::transcode(this->c_str());
	sstr.operator=(str);
	delete str;
	return sstr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *                    std::string XercesString::toString()                   *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::string XercesString::toString()
{
	const char * str = xercesc::XMLString::transcode(this->c_str());
	std::string sstr(str);
	delete str;
	return sstr;
}

} // tncfhh
