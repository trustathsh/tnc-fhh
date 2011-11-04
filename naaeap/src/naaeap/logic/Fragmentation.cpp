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
 
#include "Fragmentation.h"

#include <log4cxx/logger.h>
#include <tncs/iel/string_helper.h>

namespace tncfhh
{

namespace nal
{

/* for logging */
static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("NAA-EAP.Fragmentation"));

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Fragmentation::Fragmentation()
:active(false), maxFragmentDataSize(1000)
{
	// I didn't do it!
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Fragmentation::~Fragmentation()
{
	for (std::map<TNC_ConnectionID, std::queue<EapTncFragment *> >::iterator i = this->allEapTncFragments.begin();
			i!=this->allEapTncFragments.end();
			i++) {
		while (!i->second.empty()) {
			delete i->second.front();
			i->second.pop();
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
EapTncFragment Fragmentation::fragment(TNC_ConnectionID conID, const iel::TNCCSData &input) throw (std::logic_error)
{
	if (!this->active) {
		/* create the batch fragment */
		return EapTncFragment(input.getData(), input.getLength(), false, 0, false);
	} else {
	
		if (!this->allEapTncFragments[conID].empty()) {
			/* ERROR */
			/* Delete all fragments */
			do {
				EapTncFragment * fragment = this->allEapTncFragments[conID].front();
				delete fragment;
				this->allEapTncFragments[conID].pop();
			} while(!this->allEapTncFragments[conID].empty());
			/* delete queue */
			this->allEapTncFragments.erase(conID);
			throw std::logic_error(std::string("fragment(TNC_ConnectionID, TNCCSBatch): called while fragmentation already in progress. Cancelling fragmentation for") + conID);
		}

		/* per batch fragment one pass */
		for (TNC_UInt32 pos = 0; pos < input.getLength(); pos+=this->maxFragmentDataSize) {
			/* set length included flag */
			bool lengthIncluded = false;
			if (pos==0 && this->maxFragmentDataSize < input.getLength())
				lengthIncluded = true;

			/* set data length */
			TNC_UInt32 dataLength = 0;
			if (lengthIncluded)
				dataLength = input.getLength();

			/* set more fragments flag */
			bool moreFragments = false;
			if (pos+this->maxFragmentDataSize < input.getLength())
				moreFragments = true;

			/* calculate the length of fragment data */
			TNC_UInt32 fragmentLengthData = 0;
			if (pos+this->maxFragmentDataSize < input.getLength())
				fragmentLengthData = this->maxFragmentDataSize;
			else
				fragmentLengthData = input.getLength()-pos;

			/* calculate the pointer of fragment data */
			TNC_BufferConstReference fragmentData = input.getData()+pos;

			/* create a batch fragment */
			EapTncFragment * fragment = new EapTncFragment(fragmentData, fragmentLengthData, lengthIncluded, dataLength, moreFragments);

			LOG4CXX_DEBUG(logger, std::string("Added a fragment: lengthInclude = ") + (lengthIncluded) + ", dataLength = " + (dataLength) + ", fragmentLengthData = " + (fragmentLengthData));

			this->allEapTncFragments[conID].push(fragment);
		}
	
		return getNextFragment(conID);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
EapTncFragment Fragmentation::getNextFragment(TNC_ConnectionID conID) throw (std::logic_error)
{
	LOG4CXX_DEBUG(logger, "have " << this->allEapTncFragments[conID].size() << " fragments of ConnectionID " << conID );

	if (this->allEapTncFragments[conID].empty()) {
		/* ERROR */
		/* delete queue */
		this->allEapTncFragments.erase(conID);
		throw std::logic_error(std::string("getNextFragment(TNC_ConnectionID): called but fragmentation not already in progress. Cancelling fragmentation for") + conID);
	}
	
	/* get the next fragment */
	EapTncFragment * tmp = this->allEapTncFragments[conID].front();
	this->allEapTncFragments[conID].pop();
	
	/* if no more fragments in queue then remove queue */
	if (this->allEapTncFragments[conID].empty())
		this->allEapTncFragments.erase(conID);
	
	EapTncFragment ret = *tmp;
	delete tmp;
	return ret;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Fragmentation::reset(TNC_ConnectionID conID)
{
	if (this->allEapTncFragments.find(conID) != this->allEapTncFragments.end()) {
		/* ERROR */
        /* Delete all fragments */
		while(!this->allEapTncFragments[conID].empty()) {
			EapTncFragment * bf = this->allEapTncFragments[conID].front();
	    	delete bf;
	    	this->allEapTncFragments[conID].pop();
	    }
		/* delete queue */
		this->allEapTncFragments.erase(conID);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Fragmentation::setActive(bool active)
{
	this->active = active;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool Fragmentation::getActive()
{
	return this->active;
}


} // namespace nal

} // namespace tncfhh
