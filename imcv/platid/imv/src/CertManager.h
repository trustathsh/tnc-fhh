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
 * CertManager
 *
 * Class with functions to check if a certificate is known.
 * This is done by comparing a DN Name with a given fingerprint.
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>08.12.2009 - create class skeleton (awe)</li>
 * </ul>
 *
 * @date 08.12.09
 * @author Arne Welzel (awe)
 *
 */
#ifndef CERT_MANAGER_H_
#define CERT_MANAGER_H_

#include <iostream>



class CertManager
{
public:
	virtual ~CertManager() { /* EMPTY INLINE DEFINITON */ };
	virtual bool isCertKnown(std::string dn, std::string fingerprint) = 0;
};
#endif // CERT_MANAGER_H_
