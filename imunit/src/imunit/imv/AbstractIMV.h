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
 

#ifndef TNCFHH_IML_ABSTRACTIMV_H_
#define TNCFHH_IML_ABSTRACTIMV_H_

#include "../AbstractIMUnit.h"
#include "TNCS.h"

namespace tncfhh {

namespace iml {

// Prototyp instead of include
class IMVLibrary;

/**
 * AbstractIMV represents the abstract super class for any concrete IMV implementation. It manages
 * the current connectionState and provides helper functions for parsing message types.
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>09.05.2006 - create class (Daniel Wuttke)</li>
 *   <li>09.02.2008 - add tncs and remove all xml (mbs)</li>
 *   <li>08.05.2008 - change namespace and remove old (XML-) code (mbs)</li>
 *   <li>16.06.2009 - redesign 0.6.0 (ib)</li>
 * </ul>
 *
 * @class AbstractIMV
 * @brief Represents the abstract super class for any concrete IMV implementation.
 * @date 09.05.2006
 * @author Daniel Wuttke
 * @author Ingo Bente
 * @author Mike Steinmetz (mbs)
 */
class AbstractIMV : public AbstractIMUnit
{
    public:

    	/**
    	 * Ctor.
    	 *
    	 * @param conID The TNC_ConnectionID for the IMV instance.
    	 */
        AbstractIMV(TNC_ConnectionID conID, IMVLibrary *pImvLibrary);

        /**
         * Dtor.
         */
        virtual ~AbstractIMV();

        /**
		 * Called by TNCS when a recommendation must be provided by the IMV. IMV must call
		 * tncs.provideRecommendation().
		 *
		 * @return TNC_Result
		 */
        virtual TNC_Result solicitRecommendation();

    	/**
         * Sets the current connection state. Initializes also some attributes
         * of AbstractIMV. Calls AbstractIMUnit::notifyConnectionChange().
         *
         * @param newState The new, current connection state.
         */
         virtual TNC_Result notifyConnectionChange(TNC_ConnectionState newState);

    protected:

    	/**
    	 * The TNCS interface.
    	 * With tncs, you can call the Server.
    	 */
    	TNCS tncs;

    	/**
    	 * The calculated action recommendation.
    	 */
    	TNC_IMV_Action_Recommendation actionRecommendation;

    	/**
    	 * The calculated evaluation result.
    	 */
    	TNC_IMV_Evaluation_Result evaluationResult;

    	/**
    	 * Flag that indicates whether the validation has been finished.
    	 */
    	bool validationFinished;

    private:

};

}  // namespace iml

}  // namespace tncfhh

#endif /*TNCFHH_IML_ABSTRACTIMV_H_*/
