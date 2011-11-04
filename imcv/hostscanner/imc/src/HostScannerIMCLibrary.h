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
#ifndef HOSTSCANNERIMCLIBRARY_H_
#define HOSTSCANNERIMCLIBRARY_H_

#include <imunit/imc/IMCLibrary.h>

// FHH IANA PEN
#define TNC_VENDORID_FHH ((TNC_VendorID) 0x0080ab)
// message subtype
#define TNC_SUBTYPE_FHH_HOSTSCANNER ((TNC_MessageSubtype) 0x30)
// message type = vendorID + message subtype
#define TNC_MESSAGETYPE_FHH_HOSTSCANNER ((TNC_VENDORID_FHH << 8) | (TNC_SUBTYPE_FHH_HOSTSCANNER & 0xff))
// number of message types the IMC is interested in
#define MESSAGE_TYPE_COUNT 1

class HostScannerIMCLibrary : public tncfhh::iml::IMCLibrary{
public:
	/**
	 * Ctor. Sets all attributes.
	 */
	HostScannerIMCLibrary();

	/**
	 * Returns a pointer to a instance of DummyIMC.
	 */
	virtual tncfhh::iml::AbstractIMC *createNewImcInstance(TNC_ConnectionID conID);

	/**
	 * Dtor.
	 */
	virtual ~HostScannerIMCLibrary();
};

#endif /* HostScannerIMCLIBRARY_H_ */
