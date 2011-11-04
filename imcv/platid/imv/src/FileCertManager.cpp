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
 

#include "FileCertManager.h"
#include <iostream>
#include <fstream>

/* our logger */
static LoggerPtr
logger(Logger::getLogger("IMUnit.IMUnitLibrary.IMVLibrary.PlatidIMVLibrary.FileCertManager"));



FileCertManager::FileCertManager(const char *certfile)
{
	LOG4CXX_TRACE(logger, "FileCertManager()");
	loadCertsFromFile(certfile);
}

FileCertManager::~FileCertManager()
{
	LOG4CXX_TRACE(logger, "~FileCertManger");
}


bool FileCertManager::isCertKnown(std::string dn, std::string fingerprint)
{
	LOG4CXX_TRACE(logger, "isCertKnown()");
	LOG4CXX_DEBUG(logger, "dn = " << dn);
	LOG4CXX_DEBUG(logger, "fp = " << fingerprint);

	const map<string,string>::iterator it = certMap.find(dn);
	if (it == certMap.end()) {
		LOG4CXX_DEBUG(logger, "DN not registered");
	} else {
		LOG4CXX_DEBUG(logger, "Comparing fingerprints...");
		LOG4CXX_DEBUG(logger, "fingerprint [file]  = " << (*it).second);
		LOG4CXX_DEBUG(logger, "fingerprint [asked] = " << fingerprint);

		if (!(*it).second.compare(fingerprint)) {
			LOG4CXX_DEBUG(logger, "Good fingerprint for this dn :-)");
			return true;
		} else {
			LOG4CXX_DEBUG(logger, "Wrong fingerprint for this dn :-(");
		}
	}
	return false;
}



int FileCertManager::loadCertsFromFile(const char *certfile)
{
	LOG4CXX_INFO(logger, "Loading known certs from: " << certfile);
	string line;

	LOG4CXX_TRACE(logger, "Opening certificate file...");
	ifstream certs(certfile);
	if (certs.is_open()) {
		while (!certs.eof()) {
			getline(certs, line);
			addCertFromConfigLine(line);
		}
	} else {
		LOG4CXX_ERROR(logger, "Could not open file!");
		return -1;
	}
	LOG4CXX_INFO(logger, "Registered " << certMap.size() << " Certificates.");
	return 0;
}



int FileCertManager::addCertFromConfigLine(std::string line)
{
	LOG4CXX_TRACE(logger, "addCertFromConfigLine()");

	string dn, fp;
	unsigned int i, j;

	if (!(line.length() > 0)) {
		LOG4CXX_TRACE(logger, "Empty line?");
		return 0;
	}
	if (line.at(0) == '#') {
		LOG4CXX_TRACE(logger, "Found comment line");
		return 0;
	}

	i = line.find_first_of('"');
	if (i == line.npos) {
		LOG4CXX_WARN(logger, "Bad line! Could not find \"");
		return -1;
	}

	j = line.find_first_of('"', i+1);
	if (j == line.npos) {
		LOG4CXX_WARN(logger, "Bad line! Could not find second \"");
		return -1;
	}

	dn = line.substr(i+1, j - i -1);

	/* fingerprint should begin one space after the last "" */
	fp = line.substr(j + 2, line.length());

	LOG4CXX_TRACE(logger, "DN of line = " << dn);
	LOG4CXX_TRACE(logger, "FP of line = " << fp);

	certMap.insert(pair<string, string>(dn, fp));

	return 0;
}

