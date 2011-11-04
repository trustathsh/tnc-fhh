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
 
#include "TNCCSData.h"
#include "string_helper.h"

#include <log4cxx/logger.h>
#include <string.h>

namespace tncfhh {

namespace iel {

/* for logging */
static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("TNCS.TNCCSData"));

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNCCSData::TNCCSData(TNC_BufferReference data, TNC_UInt32 dataLength) throw (std::invalid_argument)
:data(NULL), dataLength(dataLength)
{
	LOG4CXX_TRACE(logger, "Create a TNCCSData object. length: " << dataLength << " (" << this << ")");

	if(data == NULL && dataLength != 0)
	if ( !(data!=NULL && dataLength!= 0) )
		throw std::invalid_argument(std::string("Data is NULL and/or length is 0"));

	// allocate heap memory
	this->data = new TNC_Buffer[dataLength];

	// copy data
	memcpy(this->data, data, dataLength*sizeof(TNC_Buffer));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNCCSData::TNCCSData(TNC_UInt32 dataLength)
:data(NULL), dataLength(dataLength)
{
	LOG4CXX_TRACE(logger, "Create an empty TNCCSData object. length:" << dataLength << " (" << this << ")");

	// allocate heap memory
	this->data = new TNC_Buffer[dataLength];

	memset(this->data, 0, dataLength*sizeof(TNC_Buffer));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNCCSData::TNCCSData(const TNCCSData &tnccsdata)
:data(NULL), dataLength(tnccsdata.dataLength)
{
	LOG4CXX_TRACE(logger, "Copy a TNCCSData object. length:" << tnccsdata.dataLength << " (" << this << " from " << &tnccsdata << ")");

	// allocate heap memory
	this->data = new TNC_Buffer[tnccsdata.dataLength];

	// copy data
	memcpy(this->data, tnccsdata.data, tnccsdata.dataLength*sizeof(TNC_Buffer));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNCCSData::~TNCCSData()
{
	LOG4CXX_TRACE(logger, "Delete an TNCCSData object. (" << this << ")");

	//delete data
	delete[] this->data;

	this->data = NULL;
	this->dataLength = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNCCSData& TNCCSData::operator=(TNCCSData const& assign_data)
{
	LOG4CXX_TRACE(logger, "Assign a TNCCSData object. length:" << assign_data.dataLength << " (" << this << "==" << &assign_data << ")");

	// delete old data
	delete[] this->data;
	this->data = NULL;

	//assign length
	this->dataLength = assign_data.dataLength;

	// allocate heap memory
	this->data = new TNC_Buffer[assign_data.dataLength];

	// copy data
	memcpy(this->data, assign_data.data, assign_data.dataLength*sizeof(TNC_Buffer));

	//return the result
	return *this;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_BufferConstReference TNCCSData::getData() const
{
	return this->data;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_BufferReference TNCCSData::getData()
{
	return this->data;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_UInt32 TNCCSData::getLength() const
{
	return this->dataLength;
}

} // namespace iel

} // namespace tncfhh
