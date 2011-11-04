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
#include "HostScannerIMCLibrary.h"
#include "HostScannerIMC.h"

#include <log4cxx/logger.h>

#include <tncfhhConfig.h>

using namespace log4cxx;

static LoggerPtr logger(Logger::getLogger("IMUnit.IMUnitLibrary.IMCLibrary.HostScannerIMCLibrary"));

// TNC@FHH IMCLibrary Initialization +
// include IF-IMC c-functions defined by TNC@FHH IMUnit package
TNCFHH_IMCLIBRARY_INITIALIZE(HostScannerIMCLibrary) ;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
HostScannerIMCLibrary::HostScannerIMCLibrary()
{
	LOG4CXX_INFO(logger, "Load HostScannerIMC library version " << TNCFHH_VERSION_MAJOR << "." << TNCFHH_VERSION_MINOR << "." << TNCFHH_VERSION_PATCH );
	// set all attributes inherited from tncfhh::iml::IMCLibrary
	this->ifImcImvVersion = TNC_IFIMC_VERSION_1;
	this->imUnitLibraryName = "HostScannerIMC";
	this->typeCount = MESSAGE_TYPE_COUNT;
	this->messageTypeList = new TNC_MessageType[this->typeCount];
	this->messageTypeList[0] = TNC_MESSAGETYPE_FHH_HOSTSCANNER;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
tncfhh::iml::AbstractIMC *HostScannerIMCLibrary::createNewImcInstance(TNC_ConnectionID conID)
{
	// just return a new instance of HostScannerIMC
	return new HostScannerIMC(conID, this);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
HostScannerIMCLibrary::~HostScannerIMCLibrary()
{
	delete[] this->messageTypeList;
	this->messageTypeList = NULL;
}
