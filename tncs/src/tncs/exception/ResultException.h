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
#ifndef TNCFHH_IEL_RESULTEXCEPTION_H_
#define TNCFHH_IEL_RESULTEXCEPTION_H_

#include <tcg/tnc/tncifimc.h>
#include <tcg/tnc/tncifimv.h>
#include <exception>
#include <string>

namespace tncfhh
{

namespace iel
{

/**
 * Inherits from class tncfhh::iml::Exception. Contains additional information as TNC_Result.
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>20.07.2009 - create class (ib)</li>
 * </ul>
 *
 * @class ResultException
 * @brief An exception class with TNC_Result
 * @date 07.02.2008
 * @author Ingo Bente (ib)
 * @author Mike Steinmetz (mbs)
 */
class ResultException : public std::exception
{
public:

	/**
	 * Ctor.
	 *
	 * @param message A message (as std::string) providing information regarding the exception.
	 * @param returnValue A TNC_Result value that is of importance regarding the ResultException.
	 *
	 * @todo Change order of parameters to comply with other ctor.
	 */
	ResultException(std::string message, TNC_Result returnValue) throw ();

	/**
	 * Destructor
	 */
	virtual ~ResultException() throw ();

	/**
	 * Getter for @link #returnValue @endlink
	 */
	TNC_Result getReturnValue() const;

    /** Returns a C-style character string describing the general cause
     *  of the current error.  */
    virtual const char* what() const throw();

private:
	std::string message;

	/**
	 * The returnvalue for TNCC or TNCS.
	 */
	TNC_Result returnValue;
};

} // namespace iel

} // namespace tncfhh

#endif /*TNCFHH_IEL_RESULTEXCEPTION_H_*/
