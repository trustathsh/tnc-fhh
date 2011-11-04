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
 
#ifndef CONFIGURATIONSERVICE_H
#define CONFIGURATIONSERVICE_H

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <boost/thread.hpp>

typedef std::vector<std::pair<std::string,std::string> > property_t;

namespace tncfhh {

/**
 * Singleton implementation of a configuration service object. This
 * implementation can be used as an interface to a central LDAP server.
 *
 */
class ConfigurationService {

private:

	property_t properties;


	// private default contructor since it is a singleton
	ConfigurationService() {
		readConfiguration();
	}

	// private copy contructor since it is a singleton
	ConfigurationService(ConfigurationService& service) { }

	~ConfigurationService() {}


public:

	/**
	 * retrieve the service instance
	 */
	static ConfigurationService* instance() {
		static ConfigurationService instance;
		return &instance;
	}


	/**
	 * returns the configuration properties
	 */
	property_t getProperties() const {
		return properties;
	}

protected:

	/**
	 * read the configuration properties. A few parts of the following source code
	 * are taken from the IMVHostScanner.
	 */
	std::vector<std::pair<std::string,std::string> > readConfiguration();

};

}
#endif
