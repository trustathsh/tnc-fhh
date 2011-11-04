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
 
#ifndef FRAGMENTATION_H_
#define FRAGMENTATION_H_

#include "../data/EapTncFragment.h"

#include <tncs/iel/TNCCSData.h>
#include <map>
#include <queue>

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
 *   <li>19.02.2008 - add methods (mbs)</li>
 *   <li>11.04.2008 - add reset (mbs)</li>
 *   <li>15.05.2008 - now, can disable fragmentation by compiler (mbs)</li>
 *   <li>22.07.2009 - redesign 0.6.0 (mbs)</li>
 * </ul>
 *
 * @class Fragmentation
 * @brief TODO_Brief
 * @date 16.02.2008
 * @author Ingo Bente (ib)
 * @author Mike Steinmetz (mbs)
 */

class Fragmentation
{
public:
	Fragmentation();
	virtual ~Fragmentation();
	
	/**
	 * TODO_Comment
	 *
	 * @param conID
	 * @param input
	 *
	 * @return the next fragment for send
	 */
	EapTncFragment fragment(TNC_ConnectionID conID, const iel::TNCCSData &input) throw (std::logic_error);
	
	/**
	 * TODO_Comment
	 *
	 * @param conID
	 *
	 * @return the next fragment for send
	 */
	EapTncFragment getNextFragment(TNC_ConnectionID conID) throw (std::logic_error);
	
	/**
	 * Detete all TNCCSBatchFragments for a connection ID.
	 * 
	 * @param conID The connection ID
	 */
	void reset(TNC_ConnectionID conID);

	void setActive(bool active);

	bool getActive();

private:
	/**
	 * here stored all not yet sended TNCCS-Batch-Fragments.
	 */
	std::map<TNC_ConnectionID, std::queue<EapTncFragment *> > allEapTncFragments;

	bool active;

	TNC_UInt32 maxFragmentDataSize;
};

} // namespace nal

} // namespace tncfhh

#endif /*FRAGMENTATION_H_*/
