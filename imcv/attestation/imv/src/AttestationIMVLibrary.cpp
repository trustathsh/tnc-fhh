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
 
#include "AttestationIMVLibrary.h"
#include "AttestationIMV.h"

/* logging */
#include <log4cxx/logger.h>
using namespace log4cxx;


TNCFHH_IMVLIBRARY_INITIALIZE(AttestationIMVLibrary);


static LoggerPtr
logger(Logger::getLogger("IMUnit.IMUnitLibrary.IMVLibrary.AttestationIMVLibrary"));


AttestationIMVLibrary::AttestationIMVLibrary()
{
	this->imUnitLibraryName = "AttestationIMVLibrary";
	this->addMessageType(TNC_VENDORID_FHH, TNC_SUBTYPE_FHH_ATTESTATION);
	this->policyManager = new AttestationIMVPolicyManager(IMV_CONFIG);
}

AttestationIMVLibrary::~AttestationIMVLibrary()
{
	LOG4CXX_TRACE(logger, "~AttestationIMVLibrary()");
	if (policyManager != NULL){
		delete policyManager;
		policyManager = NULL;
	}
}

AbstractIMV * AttestationIMVLibrary::createNewImvInstance(TNC_ConnectionID connid)
{
	return new AttestationIMV(connid, this, policyManager);
}
