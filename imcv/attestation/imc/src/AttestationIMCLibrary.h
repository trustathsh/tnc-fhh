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
 
/**
 * AttestationIMCLibrary
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>01.03.10 - create class skeleton (awe)</li>
 *   <li>24.03.10 - renamed to AttestationIMCLibrary (awe)</li>
 * </ul>
 *
 * @date 01.03.10
 * @author Arne Welzel (awe)
 */

#ifndef ATTESTATIONIMCLIBRARY_H_
#define ATTESTATIONIMCLIBRARY_H_

#include "tcg/tnc/tncifimc.h"
#include "imunit/imc/IMCLibrary.h"
#include <string>

#define TNC_VENDORID_FHH 		((TNC_VendorID) 0x0080ab)
/* FIXME: which MessageSubtype? */
#define TNC_SUBTYPE_FHH_ATTESTATION	((TNC_MessageSubtype) 0x34)
#define TNC_MESSAGETYPE_FHH_ATTESTATION ((TNC_VENDORID_FHH << 8) | (TNC_SUBTYPE_FHH_ATTESTATION & 0xff))


using namespace tncfhh::iml;
using namespace std;

class AttestationIMCLibrary : public tncfhh::iml::IMCLibrary
{
public:
	AttestationIMCLibrary();

	AbstractIMC *createNewImcInstance(TNC_ConnectionID conId);
};

#endif // ATTESTATIONIMCLIBRARY_H_
