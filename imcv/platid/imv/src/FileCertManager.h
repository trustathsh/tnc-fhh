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
 * FileCertManager
 *
 * Implementation of the CertManager Class
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>08.12.2009 - create class skeleton and some logic (awe)</li>
 *   <li>16.12.2009 - added missing ifdef endif construction (awe)</li>
 * </ul>
 *
 * @date 08.12.09
 * @author Arne Welzel (awe)
 *
 */
#ifndef FILE_CERT_MANAGER_H_
#define FILE_CERT_MANAGER_H_

#include "CertManager.h"
#include <string>

/* log4cxx includes */
#include "log4cxx/logger.h"
using namespace log4cxx;
using namespace std;

class FileCertManager : public CertManager
{
private:
	/* map with certs */
	std::map<string, string> certMap;

	/* returns -1 on failure, 0 on success */
	int loadCertsFromFile(const char *certfile);

	/* parses one line from the cert and adds if it's good */
	int addCertFromConfigLine(std::string line);

public:
	/* constructor */
	FileCertManager(const char *certfile);

	/* destructor */
	virtual ~FileCertManager();

	/* implementation of abstract method */
	virtual bool isCertKnown(std::string dn, std::string fingerprint);
};

#endif //FILE_CERT_MANAGER_H
