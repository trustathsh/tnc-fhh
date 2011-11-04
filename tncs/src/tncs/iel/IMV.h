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
#ifndef TNCS_IMV_H_
#define TNCS_IMV_H_

#include "IMVProperties.h"

#include <list>

namespace tncfhh {

namespace iel {

/**
 * Connection specific IMV instance.
 * 
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>25.02.2008 - create class (mbs)</li>
 *   <li>28.02.2008 - impl methods (mbs)</li>
 *   <li>08.04.2008 - fixed a hasProvideRecommendation() bug (mbs)</li>
 *   <li>15.05.2008 - add error handling (mbs)</li>
 *   <li>13.07.2009 - redesign for 0.6.0 (mbs)</li>
 * </ul>
 *
 * @class IMV
 * @brief TODO_Brief
 * @date 25.02.2008
 * @author Mike Steinmetz (mbs)
 */
class IMV
{
public:
	/**
	 * Constructor. Calls TNC_IMV_NotifyConnectionChange(CREATE).
	 *
	 * @param conID the Connection ID for this IMV instance
	 * @param imvProperties Reference to IMVProperties instance
	 */
	IMV(TNC_ConnectionID conID, IMVProperties *imvProperties);

	/**
	 * Destructor
	 */
	virtual ~IMV();
	
	virtual void notifyConnectionChange(TNC_ConnectionState connectionState);

	/**
	 * Forwards the call to IMVProperties.
	 */
	virtual void receiveMessage(TNC_BufferReference message, TNC_UInt32 messageLength, TNC_MessageType messageType);
	virtual void solicitRecommendation();
	virtual void batchEnding();
	
	virtual bool isInNotifyConnectionChange();
	virtual bool isInReceiveMessage();
	virtual bool isInSolicitRecommendation();
	virtual bool isInBatchEnding();

	virtual bool hasProvideRecommendation();
	virtual void setProvideRecommendation(TNC_IMV_Action_Recommendation recommendation, TNC_IMV_Evaluation_Result evaluation);
	
	/**
	 * TODO_Comment
	 * <br>
	 * call only function if hasProvideRecommendation() return true
	 *
	 * @return the action recommendation
	 */
	virtual TNC_IMV_Action_Recommendation getRecommendation();
	
	/**
	 * TODO_Comment
	 * <br>
	 * call only function if hasProvideRecommendation() return true
	 *
	 * @return the evaluation result
	 */
	virtual TNC_IMV_Evaluation_Result getEvaluation();
	
	/**
	 * TODO_Comment
	 *
	 * @return a const reference to IMVProperties instance (don't delete that reference)
	 */
	virtual const IMVProperties *getIMVProperties();

	/**
	 * return if the IMV has an error. The error is resetting on begin handshake.
	 *
	 * @return true if the imv has an error, otherwise false
	 */
	virtual bool hasError() const;
	
	TNC_ConnectionID getConnectionID();


private:
	TNC_ConnectionID conID;
	IMVProperties *imvProperties;
	
	bool inNotifyConnectionChange;
	bool inReceiveMessage;
	bool inSolicitRecommendation;
	bool inBatchEnding;
	
	bool isProvideRecommendationSet;
	bool error;
	TNC_IMV_Action_Recommendation recommendation;
	TNC_IMV_Evaluation_Result evaluation;

	void handleOtherError();
};

}  // namespace iel

}  // namespace tncfhh

#endif /*TNCS_IMV_H_*/
