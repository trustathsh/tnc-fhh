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
 

#include "AbstractIMV.h"

#include <log4cxx/logger.h>

using namespace log4cxx;

namespace tncfhh {

namespace iml {

static LoggerPtr logger(Logger::getLogger("IMUnit.AbstractIMUnit.AbstractIMV"));

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
AbstractIMV::AbstractIMV(TNC_ConnectionID conID, IMVLibrary *pImvLibrary)
	:AbstractIMUnit(conID), tncs(conID, pImvLibrary),
	actionRecommendation(TNC_IMV_ACTION_RECOMMENDATION_NO_RECOMMENDATION),
	evaluationResult(TNC_IMV_EVALUATION_RESULT_ERROR),
	validationFinished(false)
{
    // intentionally left blank
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
AbstractIMV::~AbstractIMV()
{
    // intentionally left blank
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result AbstractIMV::notifyConnectionChange(TNC_ConnectionState newState)
{
	LOG4CXX_TRACE(logger, "notifyConnectionChange new state = " << newState);

    if (newState == TNC_CONNECTION_STATE_HANDSHAKE || newState == TNC_CONNECTION_STATE_CREATE) {
        actionRecommendation = TNC_IMV_ACTION_RECOMMENDATION_NO_RECOMMENDATION;
        evaluationResult = TNC_IMV_EVALUATION_RESULT_DONT_KNOW;
        validationFinished = false;
    }

    return AbstractIMUnit::notifyConnectionChange(newState);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result AbstractIMV::solicitRecommendation()
{
	LOG4CXX_TRACE(logger, "solicitRecommendation");

    tncs.provideRecommendation(this->actionRecommendation, this->evaluationResult);

	return TNC_RESULT_SUCCESS;
}

}  // namespace iml

}  // namespace tncfhh
