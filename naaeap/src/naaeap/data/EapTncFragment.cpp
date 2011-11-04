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
 
#include "EapTncFragment.h"

#include <netinet/in.h>
#include <string.h>
#include <log4cxx/logger.h>
#include <tncs/iel/string_helper.h>


namespace tncfhh
{

namespace nal
{

/* for logging */
static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("NAA-EAP.EapTncFragment"));

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
EapTncFragment::EapTncFragment(const TNC_BufferReference fragment, TNC_UInt32 fLength) throw (std::invalid_argument)
	:fragment(NULL), fragmentLength(fLength)
{
	LOG4CXX_TRACE(logger, "Create EapTncFragment from EAP-TNC Packet " << this)

	// check parameters
	if(fragment == NULL || fLength < EAP_TNC_HEADER_FLAGS_SIZE) {
		throw std::invalid_argument(std::string("Invalid Parameters: fragmentData = ") + ((void*)fragment) + ", fLength = " + (fLength) + ".");
	}

	/* read relevant TNC flags */
	this->lengthIncluded = (*fragment)&0x80             /*10000000*/;
	this->moreFragments  = (*fragment)&0x40             /*01000000*/;

	/* check advanced parameter */
	if (this->lengthIncluded && fLength < (EAP_TNC_HEADER_FLAGS_SIZE + EAP_TNC_HEADER_DATA_LENGTH_SIZE) )
		throw std::invalid_argument(std::string("Invalid Parameters: length is included but fLength = ") + (fLength) + ".");

	/* calculate attributes */
	if (this->lengthIncluded){
		this->TNCCSDataLength = ntohl(*(unsigned long int *)(fragment+1));
	} else {
		this->TNCCSDataLength = 0;
	}

	/* copy fragment */
	this->fragment = new TNC_Buffer[fLength];
	memcpy(this->fragment, fragment, fLength*sizeof(TNC_Buffer));

	LOG4CXX_TRACE(logger, "LengthIncludet: " << this->lengthIncluded <<
			" moreFragments: " << this->moreFragments <<
			" TNCCSDataLength: " << this->TNCCSDataLength <<
			" dataLength: " << this->getDataLength());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
EapTncFragment::EapTncFragment(TNC_BufferConstReference data, TNC_UInt32 dataLength, bool lengthIncluded, TNC_UInt32 TNCCSDataLength, bool moreFragments) throw (std::invalid_argument)
	:fragment(NULL), fragmentLength(0), TNCCSDataLength(0), lengthIncluded(lengthIncluded), moreFragments(moreFragments)
{
	LOG4CXX_TRACE(logger, "Create EapTncFragment from TNCCSData " << this)

	/* check parameters */
	if( (data == NULL && dataLength != 0) ||
		(data != NULL && dataLength == 0) || (data ==NULL && dataLength == 0) )
		throw std::invalid_argument(std::string("Invalid Parameters: data = ") + (data) + ", dataLength = " + (dataLength) + ".");

	/* check more parameters */
	if (this->lengthIncluded && TNCCSDataLength == 0)
		throw std::invalid_argument(std::string("Invalid Parameters: lengthIncluded = true, TNCCSDataLength = ") + (TNCCSDataLength) + ".");

	/* check still more parameters */
	if (this->moreFragments && this->lengthIncluded && dataLength >= TNCCSDataLength)
		throw std::invalid_argument(std::string("Invalid Parameters: lengthIncluded = true, moreFragemnts = true, TNCCSDataLength = ") + (TNCCSDataLength) + ", dataLength = " + (dataLength) + ".");

	/* set fragmentLength */
	if (this->lengthIncluded)
		this->fragmentLength = dataLength + (EAP_TNC_HEADER_FLAGS_SIZE + EAP_TNC_HEADER_DATA_LENGTH_SIZE);
	else
		this->fragmentLength = dataLength + EAP_TNC_HEADER_FLAGS_SIZE;

	/* allocate memory */
	this->fragment = new TNC_Buffer[sizeof(TNC_Buffer)*this->fragmentLength];

	/* set version in fragment */
	*this->fragment = 0x01;
	/* add length included bit */
	*this->fragment |= (this->lengthIncluded?0x80:0x00);
	/* add more fragments bit */
	*this->fragment |= (this->moreFragments?0x40:0x00);

	/* set TNCCS-Data length */
	if (this->lengthIncluded) {
		/* in fragment */
		*(unsigned long int *)(this->fragment+1) = htonl(TNCCSDataLength);
		/* in local variable */
		this->TNCCSDataLength = TNCCSDataLength;
	}

	/* copy data */
	memcpy(this->lengthIncluded?this->fragment+(EAP_TNC_HEADER_FLAGS_SIZE + EAP_TNC_HEADER_DATA_LENGTH_SIZE):this->fragment+EAP_TNC_HEADER_FLAGS_SIZE, data, sizeof(TNC_Buffer)*dataLength);

	LOG4CXX_TRACE(logger, "LengthIncludet: " << this->lengthIncluded <<
			" moreFragments: " << this->moreFragments <<
			" TNCCSDataLength: " << this->TNCCSDataLength <<
			" dataLength: " << this->getDataLength());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
EapTncFragment::EapTncFragment(const EapTncFragment & source)
:fragment(NULL), fragmentLength(source.fragmentLength),
TNCCSDataLength(source.TNCCSDataLength), lengthIncluded(source.lengthIncluded),
moreFragments(source.moreFragments)
{
	LOG4CXX_TRACE(logger, "Create EapTncFragment from other EapTncFragment " << &source << " " << this);

	this->fragment = new TNC_Buffer[fragmentLength];
	memcpy(this->fragment, source.fragment, fragmentLength*sizeof(TNC_Buffer));
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
EapTncFragment::~EapTncFragment()
{
	LOG4CXX_TRACE(logger, "Delete me. " << this);

	delete this->fragment;
	this->fragment = NULL;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
EapTncFragment & EapTncFragment::operator=(const EapTncFragment & source)
{
	LOG4CXX_TRACE(logger, this << "=" << &source);

	this->TNCCSDataLength = source.TNCCSDataLength;
	this->fragmentLength = source.fragmentLength;
	this->lengthIncluded = source.lengthIncluded;
	this->moreFragments = source.moreFragments;

	delete this->fragment;
	this->fragment = new TNC_Buffer[source.fragmentLength];
	memcpy(this->fragment, source.fragment, source.fragmentLength*sizeof(TNC_Buffer));

	return *this;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_BufferReference EapTncFragment::getData() const
{
	if (this->lengthIncluded){
		return this->fragment + (EAP_TNC_HEADER_FLAGS_SIZE + EAP_TNC_HEADER_DATA_LENGTH_SIZE);
	} else {
		return this->fragment  + EAP_TNC_HEADER_FLAGS_SIZE;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_UInt32 EapTncFragment::getDataLength() const
{
	/* calculate attributes */
	if (this->lengthIncluded){
		return this->fragmentLength - (EAP_TNC_HEADER_FLAGS_SIZE + EAP_TNC_HEADER_DATA_LENGTH_SIZE);
	} else {
		return this->fragmentLength - (EAP_TNC_HEADER_FLAGS_SIZE);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */#
TNC_BufferConstReference EapTncFragment::getFragment() const
{
	return this->fragment;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_UInt32 EapTncFragment::getFragmentLength() const
{
	return this->fragmentLength;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_UInt32 EapTncFragment::getTNCCSDataLength() const
{
	return this->TNCCSDataLength;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool EapTncFragment::hasLengthIncluded() const
{
	return this->lengthIncluded;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool EapTncFragment::hasMoreFragments() const
{
	return this->moreFragments;
}

} // namespace nal

} // namespace tncfhh
