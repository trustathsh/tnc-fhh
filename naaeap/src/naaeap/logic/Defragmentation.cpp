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
 
#include "Defragmentation.h"

#include <log4cxx/logger.h>
#include <tncs/iel/string_helper.h>

namespace tncfhh
{

namespace nal
{

/* for logging */
static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("NAA-EAP.Defragmentation"));

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Defragmentation::Defragmentation()
{
	// Notting(ham)
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Defragmentation::~Defragmentation()
{
	// Bjorn Forest
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
iel::TNCCSData Defragmentation::defragment(TNC_ConnectionID conID, const EapTncFragment &input) throw (std::logic_error, AcknowledgementException)
{
	LOG4CXX_DEBUG(logger, "Incomming Fragment for ConnectionID " << conID << " (" << input.getFragmentLength() << " Bytes with " << input.getDataLength() << " Bytes logical Data)");
	
	try {
		// check parameters
		checkParameters(conID, input);
		
		// process fragment according to L/M flags
		if(!input.hasLengthIncluded() && !input.hasMoreFragments()){
			// no length included + no more fragments ( 0 0 ) -> last or only fragment
			LOG4CXX_DEBUG(logger, "no length included + no more fragments ( 0 0 ) -> last or only fragment");
			return handleLastOrOnlyFragment(conID, input);
		} else if(!input.hasLengthIncluded() && input.hasMoreFragments()){
			// no length included + more fragments    ( 0 1 ) -> middle fragment
			LOG4CXX_DEBUG(logger, "no length included + more fragments    ( 0 1 ) -> middle fragment");
			handleMiddleFragment(conID, input);
			// exception for send ACK back
			throw AcknowledgementException();
		} else if(input.hasLengthIncluded() && !input.hasMoreFragments()){
			// length included +  no more fragments   ( 1 0 ) -> first and only fragment
			LOG4CXX_DEBUG(logger, "length included +  no more fragments   ( 1 0 ) -> first and only fragment");
			return handleFirstAndOnlyFragment(conID, input);
		} else if(input.hasLengthIncluded() && input.hasMoreFragments()){
			// length included + more fragments       ( 1 1 ) -> first fragment + more to follow
			LOG4CXX_DEBUG(logger, "length included + more fragments       ( 1 1 ) -> first fragment + more to follow");
			handleFirstFragment(conID, input);
			// exception for send ACK back
			throw AcknowledgementException();
		} else
			throw std::logic_error("WTF is wrong? I must not throw!");
	} catch (std::logic_error & e) {
		LOG4CXX_ERROR(logger, e.what());
		if(existsTNCCSBatchBuffer(conID)){
			LOG4CXX_ERROR(logger, "Deleting Fragmentation data for conID " << (conID));
			// delete buffer object
			delete allTNCCSBatchBuffers[conID];
			// delete buffer from map
			allTNCCSBatchBuffers.erase(conID);
		}
		throw (e);		
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Defragmentation::reset(TNC_ConnectionID conID)
{
	if (existsTNCCSBatchBuffer(conID)) {
		// delete buffer object
		delete allTNCCSBatchBuffers[conID];
		// delete buffer from map
		allTNCCSBatchBuffers.erase(conID);
	}		
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
iel::TNCCSData Defragmentation::handleLastOrOnlyFragment(TNC_ConnectionID conID, const EapTncFragment &input) throw (std::logic_error)
{
	if (!existsTNCCSBatchBuffer(conID)) {
		// no fragmentation in process -> last and only fragment
		return iel::TNCCSData(input.getData(), input.getDataLength());
	} else {
		EapTncBuffer *buffer = NULL;
		// fragmentation in process -> get EapTncBuffer for conID
		buffer = allTNCCSBatchBuffers[conID];
		// add last TNCCSBatchFragment to buffer
		buffer->addEapTncFragment(input, true);
		// get TNCCSData
		iel::TNCCSData ret = buffer->getTNCCSData();
		// delete buffer from map
		allTNCCSBatchBuffers.erase(conID);
		// delete buffer object
		delete buffer;
		return ret;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Defragmentation::handleMiddleFragment(TNC_ConnectionID conID, const EapTncFragment &input) throw (std::logic_error)
{
	// fragmentation in process -> get tnccsBatchBuffer for conID
	EapTncBuffer * batchBuffer = allTNCCSBatchBuffers[conID];
	// add TNCCSBatchFragment to buffer
	batchBuffer->addEapTncFragment(input, false);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
iel::TNCCSData Defragmentation::handleFirstAndOnlyFragment(TNC_ConnectionID conID, const EapTncFragment &input) throw (std::logic_error)
{
	// allocate new TNCCSBatch
	return iel::TNCCSData(input.getData(), input.getDataLength());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Defragmentation::handleFirstFragment(TNC_ConnectionID conID, const EapTncFragment &input) throw (std::logic_error)
{
	// start fragmentation process
	// allocate new batchBuffer
	EapTncBuffer * batchBuffer = new EapTncBuffer(input);
	// add buffer to map
	allTNCCSBatchBuffers[conID] = batchBuffer;
}

bool Defragmentation::existsTNCCSBatchBuffer(TNC_ConnectionID conID) const
{
	return this->allTNCCSBatchBuffers.find(conID) != this->allTNCCSBatchBuffers.end();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Defragmentation::checkParameters(TNC_ConnectionID conID, const EapTncFragment &input) throw (std::logic_error)
{
	// length included + more fragments -> first fragment + more to follow
	if(input.hasLengthIncluded() && input.hasMoreFragments()){
		// check that fragmentation process has not already started
		if(existsTNCCSBatchBuffer(conID)){
			throw std::logic_error(std::string("Received first fragment for conID where fragmentation is already in progress! conID = ") + conID);
		}
	}
	
	// length included +  no more fragments -> first and only fragment
	if(input.hasLengthIncluded() && !input.hasMoreFragments()){
		// check that no fragmentation process is going on
		if(existsTNCCSBatchBuffer(conID)){
			throw std::logic_error(std::string("Received first and only fragment but fragmentation process is going on for conID = ") + (conID));
		}
	}
	
	// no length included + more fragments -> middle fragment
	if(!input.hasLengthIncluded() && input.hasMoreFragments()){
		// check that fragmentation process is going on
		if(!existsTNCCSBatchBuffer(conID)){
			throw std::logic_error(std::string("Received middle fragment for conID where fragmentation is NOT in progress! conID = ") + (conID));
		}
	}
	
	// no length included + no more fragments -> only or last fragment
	// - allTNCCSBatchBuffers[conID] can be empty or not. No exception.
}

} // namespace nal

} // namespace tncfhh
