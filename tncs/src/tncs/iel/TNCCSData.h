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
 
#ifndef TNCS_TNCCSDATA_H_
#define TNCS_TNCCSDATA_H_

#include <tcg/tnc/tncifimv.h>
#include <tcg/tnc/fhhaddon.h>
#include <stdexcept>

namespace tncfhh
{

namespace iel
{

/**
 * Represent TNCCS-Data message.
 * 
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>16.02.2008 - create class (ib)</li>
 *   <li>19.02.2008 - add some comments and friend (mbs)</li>
 *   <li>29.02.2008 - the last functionality ;) (mbs)</li>
 *   <li>12.07.2009 - change to TNCCSData (mbs)</li>
 * </ul>
 *
 * @class TNCCSData
 * @brief a complete TNCCS-Data message
 * @date 16.02.2008
 * @author Ingo Bente (ib)
 * @author Mike Steinmetz (mbs)
 */
class TNCCSData
{
public:
	
	/**
	 * Constructor for one complete TNCCSData.<br/>
	 * (call by value and deep copy from parameters).
	 *
	 * @param data Pointer to the data data
	 * @param dataLength Length of the data data
	 */
	TNCCSData(TNC_BufferReference data, TNC_UInt32 dataLength) throw (std::invalid_argument);
	
	/**
	 * Copy constructor. (deep copy)
	 *
	 * @param TNCCSData An TNCCSData
	 */
	TNCCSData(const TNCCSData &TNCCSData);

	/**
	 * Constructor that create an empty object.
	 *
	 * @param length the length of the complete TNCCS-data
	 */
	TNCCSData(TNC_UInt32 length);


	/**
	 * Destructor
	 */
	virtual ~TNCCSData();
	

	/*
	 * Operator for data=assign_data
	 *
	 * @param assign_data that assign to this
	 */
    TNCCSData& operator=(TNCCSData const& assign_data);

	/**
	 * Get the const TNCCS-data
	 *
	 * @return the data
	 *
	 * @see getLength()
	 */
	TNC_BufferConstReference getData() const;

	/**
	 * Get the TNCCS-data
	 *
	 * @return the data
	 * 
	 * @see getLength()
	 */
	TNC_BufferReference getData();
	
	/**
	 * Get the length of TNCCS-data
	 *
	 * @return the length of data
	 * 
	 * @see getdata()
	 */
	TNC_UInt32 getLength() const;
	
private:
	/**
	 * The TNCCS-Data
	 */
	TNC_BufferReference data;
	
	/**
	 * The length of data
	 */
	TNC_UInt32 dataLength;
};

} // namespace iel

} // namespace tncfhh

#endif /*TNCS_TNCCSDATA_H_*/
