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
 
#ifndef DEFRAGMENTATION_H_
#define DEFRAGMENTATION_H_

#include "../data/EapTncBuffer.h"
#include "../exception/AcknowledgementException.h"

#include <map>

namespace tncfhh
{

namespace nal
{

/**
 * TODO_Comment
 * 
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>16.02.2008 - create class (ib)</li>
 *   <li>11.04.2008 - add reset (mbs)</li>
 *   <li>22.07.2009 - redesign 0.6.0 (mbs)</li>
 * </ul>
 *
 * @class Defragmentation
 * @brief TODO_Brief
 * @date 16.02.2008
 * @author Ingo Bente (ib)
 * @author Mike Steinmetz (mbs)
 */
class Defragmentation
{
public:
	Defragmentation();
	virtual ~Defragmentation();
	
	/**
	 * Handle an incoming TNCCSMessageFragment.
	 *
	 * @param conID The connection ID
	 * @param input Fragment received from the peer.
	 *
	 * @return If input is the last fragment, the defragmented TNCCS message. Else an ACK.
	 */
	iel::TNCCSData defragment(TNC_ConnectionID conID, const EapTncFragment &input) throw (std::logic_error, AcknowledgementException);

	/**
	 * Detete all TNCCSBatchFragments for a connection ID.
	 * 
	 * @param conID The connection ID
	 */
	void reset(TNC_ConnectionID conID);
	
private:
	/**
	 * Handles the last or only TNCCSBatchFragment. ( 0 0 )
	 *
	 * @param conID The connectionID.
	 * @param input The incoming TNCCSBatchFragment.
	 *
	 * @return pointer to a TNCCSBatch. NULL == ACK.
	 */
	iel::TNCCSData handleLastOrOnlyFragment(TNC_ConnectionID conID, const EapTncFragment &input) throw (std::logic_error);
	
	/**
	 * Handles the middle TNCCSBatchFragment. ( 0 1 )
	 *
	 * @param conID The connectionID.
	 * @param input The incoming TNCCSBatchFragment.
	 *
	 * @return pointer to a TNCCSBatch. NULL == ACK.
	 */
	void handleMiddleFragment(TNC_ConnectionID conID, const EapTncFragment &input) throw (std::logic_error);
	
	/**
	 * Handles the first and only TNCCSBatchFragment. ( 1 0 )
	 *
	 * @param conID The connectionID.
	 * @param input The incoming TNCCSBatchFragment.
	 *
	 * @return pointer to a TNCCSBatch. NULL == ACK.
	 */
	iel::TNCCSData handleFirstAndOnlyFragment(TNC_ConnectionID conID, const EapTncFragment &input) throw (std::logic_error);
	
	/**
	 * Handles the first TNCCSBatchFragment. ( 1 1 )
	 *
	 * @param conID The connectionID.
	 * @param input The incoming TNCCSBatchFragment.
	 *
	 * @return pointer to a TNCCSBatch. NULL == ACK.
	 */
	void handleFirstFragment(TNC_ConnectionID conID, const EapTncFragment &input) throw (std::logic_error);
	
	/**
	 * Checks if an entry for conID exists. 
	 *
	 * @param conID A TNC_ConnectionID
	 *
	 * @return true, if entry is present, else false.
	 */
	bool existsTNCCSBatchBuffer(TNC_ConnectionID conID) const;
	
	/**
	 * Checks if the parameters and the current state of the defragmentation object match.
	 *
	 * @param conID The connection ID
	 * @param input The incoming TNCCSBatchFragment
	 *
	 */
	void checkParameters(TNC_ConnectionID conID, const EapTncFragment &input) throw (std::logic_error);
	
	/**
	 * Container that manages all TNCCSBatchBuffers for all connectionIDs.
	 */
	std::map<TNC_ConnectionID, EapTncBuffer*> allTNCCSBatchBuffers;
};

} // namespace nal

} // namespace tncfhh

#endif /*DEFRAGMENTATION_H_*/
