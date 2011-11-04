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
#ifndef TNCS_POLICY_H_
#define TNCS_POLICY_H_

#include "IMV.h"

#include <list>

namespace tncfhh {

namespace iel {

/**
 * TODO_Comment
 * 
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>25.02.2008 - create class (mbs)</li>
 *   <li>29.02.2008 - impl methods (mbs)</li>
 *   <li>08.04.2008 - fixed the accessIsolated bug (mbs)</li>
 *   <li>11.04.2008 - added resetPolicy [bug] (mbs)</li>
 *   <li>16.05.2008 - inform IMVs about notifyConnectionChange [bug] (mbs)</li>
 *   <li>16.07.2009 - redesign 0.6.0 (ib)</li>
 * </ul>
 *
 * @class Policy
 * @brief TODO_Brief
 * @date 25.02.2008
 * @author Mike Steinmetz (mbs)
 *         Ingo Bente (ib)
 */
class Policy
{
public:
	Policy();
	virtual ~Policy();
	
	/**
	 * Reset the Policy. In this Implementation the method do nothing.
	 */
	virtual void resetPolicy();
	
	/**
	 * Computes an overall action recommendation from the recommendations
	 * provided by the single IMV instances. This function works as follows:
	 * - if all IMVs provide an allow -> an allow is returned
	 * - if there is at least one none -> a none is returned
	 * - if there is at least one isolate and no none -> an isolate is returned
	 *
	 * In short: allow < isolate < none.
	 * The evaluation results provided by the IMVs are not considered.
	 *
	 * This function is called when the IF-TNCCS implementation notifies that
	 * a handshake is finished.
	 *
	 * TODO - getActionRecommendation make it configurable
	 */
	virtual TNC_IMV_Action_Recommendation getActionRecommendation(const std::list<IMV *> &imvs);

	/**
	 * TODO_Comment
	 *
	 * @param outgoingTnccsBatch
	 * @param imvs
	 *
	 * @throw ConnectionStateException is handshake Finished
	 */
//	virtual void isHandshakeFinished(TNCCSBatch & outgoingTnccsBatch, std::list<IMV*> imvs); TODO - Policy move isHandshakeFinished to IF-TNCS Protocol
	
private:
//	void handshakeIsFinished(TNCCSBatch & outgoingTnccsBatch,  std::list<IMV*> & imvs); TODO - Policy - create recommendation message in IF-TNCS Protocol
};

}  // namespace iel

}  // namespace tncfhh

#endif /*TNCS_POLICY_H_*/
