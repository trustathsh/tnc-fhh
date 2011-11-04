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
 
#ifndef TNCS_CONNECTIONSTATEEXCEPTIONWITHTNCCSDATA_H_
#define TNCS_CONNECTIONSTATEEXCEPTIONWITHTNCCSDATA_H_

#include "ConnectionStateException.h"
#include "../iel/TNCCSData.h"
#include <tcg/tnc/tncifimv.h>

namespace tncfhh
{

namespace iel
{

/**
 * Exception that has information about the TNC_ConnectionState and provides
 * the last outgoing TNCCSData.
 * 
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>19.02.2008 - create class (ib)</li>
 *   <li>19.02.2008 - add getter (mbs)</li>
 *   <li>29.02.2008 - move in NAA-TNCS (mbs)</li>
 *   <li>17.07.2009 - redesign 0.6.0 (ib)</li>
 *   <li>10.09.2009 - add method what (mbs)</li>
 * </ul>
 *
 * @class ConnectionStateExceptionWithTNCCSData
 * @brief Exception that has information about the TNC_ConnectionState.
 * @date 19.02.2008
 * @author Ingo Bente (ib)
 * @author Mike Steinmetz (mbs)
 */
class ConnectionStateExceptionWithTNCCSData : public ConnectionStateException
{
public:
	
	/**
	 * Constructor
	 * 
	 * @param message An error message.
	 * @param connectionState An TNC_ConnectionState.
	 */
	ConnectionStateExceptionWithTNCCSData(TNC_ConnectionState connectionState, TNCCSData lastTnccsData) throw ();
	
	/**
	 * Destructor
	 */
	virtual ~ConnectionStateExceptionWithTNCCSData() throw ();
	
	/**
	 * Returns the last TnccsData
	 */
	TNCCSData getLastTnccsData() const;

	/**
	 * overwrite the std::exception method what
	 *
	 * @return get info about this exception
	 */
    virtual const char* what() const throw();

private:
	
	/**
	 * Last outgoing TNCCSData from TNCS to TNCC
	 */
	TNCCSData lastTnccsData;

};

} // namespace iel

} // namespace tncfhh

#endif /*TNCS_CONNECTIONSTATEEXCEPTIONWITHTNCCSDATA_H_*/
