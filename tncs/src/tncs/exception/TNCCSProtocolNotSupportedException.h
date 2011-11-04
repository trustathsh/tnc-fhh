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
 
#ifndef TNCS_TNCCSPROTOCOLNOTSUPPORTEDEXCEPTION_H_
#define TNCS_TNCCSPROTOCOLNOTSUPPORTEDEXCEPTION_H_

#include <tcg/tnc/tncifimv.h>
#include <exception>
#include <string>

namespace tncfhh
{

namespace iel
{

/**
 * Exception representing that a certain TNCCS protocol is not supported.
 * 
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>17.07.2009 - create class (ib)</li>
 * </ul>
 *
 * @date 17.07.2009
 * @author Ingo Bente (ib)
 */
class TNCCSProtocolNotSupportedException : public std::exception
{
public:
	
	/**
	 * Constructor
	 * 
	 * @param message An error message.
	 */
	TNCCSProtocolNotSupportedException(std::string message) throw ();
	
	/**
	 * Destructor
	 */
	virtual ~TNCCSProtocolNotSupportedException() throw ();

    /** Returns a C-style character string describing the general cause
     *  of the current error.  */
    virtual const char* what() const throw();

private:
	std::string message;
	
};

} // namespace iel

} // namespace tncfhh

#endif /*TNCS_TNCCSPROTOCOLNOTSUPPORTEDEXCEPTION_H_*/
