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
 
/*
 * Configuration.h
 *
 *  Created on: 16.02.2010
 *      Author: Alexander Schulz
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

namespace tncfhh {

class Configuration {

private:
	std::map<std::string,std::string> map;

	// private default contructor since it is a singleton
	Configuration() {
	}

	// private copy contructor since it is a singleton
	Configuration(Configuration& config) { }

	~Configuration() {}

public:
	void addConfigurationFile(std::string filename);

	void set(std::string key, std::string value);

	std::string get(std::string key);

	/**
	 * retrieve the config instance
	 */
	static Configuration* instance();
};

}

#endif /* CONFIGURATION_H_ */
