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
 
#include "ConfigurationService.h"
#include "Configuration.h"

namespace tncfhh {

std::vector<std::pair<std::string,std::string> > ConfigurationService::readConfiguration() {

	std::ifstream file;
	char buffer[1024];

	std::string filename = Configuration::instance()->get("CONFIGURATION_SERVICE_FILE");
	file.open(filename.c_str(), std::ios::in);

	if (file.good()) {
		file.seekg(0L, std::ios::beg);

		while (!file.eof()) {
			file.getline(buffer, 1024);
			std::string line = std::string(buffer);

			/* ignore comments */
			if (line.size()<1 || line.at(0)=='#')
				continue;

			size_t pos = line.find_first_of('=');

			/* ignore illegal lines */
			if (pos == std::string::npos) {
				continue;
			}

			/* get key */
			std::string key = line.substr(0, pos);

			/* get value */
			std::string value = line.substr(pos + 1, line.size());

	        /* add key-value-pair */
			properties.push_back(std::pair<std::string, std::string>(key, value));

		}
	}
	else {
		std::cout << "File not found!" << std::endl;
	}
    return properties;
}


}
