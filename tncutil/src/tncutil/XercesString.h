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
#ifndef XERCESSTRING_H_
#define XERCESSTRING_H_

#include <string>
#include <xercesc/util/XercesDefs.hpp>

namespace tncfhh
{

/**
 * Class for mapping between C/C++ strings and Xerces Strings.
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>26.02.2008 - create class (mbs)</li>
 *   <li>20.07.2009 - change namespace (mbs)</li>
 * </ul>
 *
 * @class XercesString
 * @date 26.02.2008
 * @author Mike Steinmetz (mbs)
 */
class XercesString : public std::basic_string<XMLCh>
{
public:

	/**
	 * Ctor.
	 *
	 * @param const char*
	 *
	 */
	XercesString(const char * str);

	/**
	 * Ctor.
	 *
	 * @param const std::string&
	 *
	 */
	XercesString(const std::string & str);

	/**
	 * Ctor.
	 *
	 * @param const XMLCh*
	 *
	 */
	XercesString(const XMLCh * xstr);

	/**
	 * Dtor.
	 *
	 * @param
	 *
	 */
	virtual ~XercesString();

	/**
	 * Operator=
	 *
	 * @param xstr
	 *
	 * @return return_type
	 */
	virtual XercesString & operator=(const XMLCh * xstr);

	/**
	 * Copy content of this to sstr.
	 *
	 * @param sstr
	 *
	 * @return return_type
	 */
	virtual std::string & copyToString(std::string & sstr);

	/**
	 * Get a std::string.
	 *
	 * @return return_type
	 */
	virtual std::string toString();
};

} // tncfhh

#endif /*TNCS_XERCESSTRING_H_*/
