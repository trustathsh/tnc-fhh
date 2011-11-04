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
 

#include "../naavpn/TNCServer.h"
#include <tncutil/Configuration.h>

#include <tncs/iel/Coordinator.h>

/**
 * main function of the NAL-TLS enabled Policy Decision Point.
 */
int main(int argc, char* argv[]) {

	// We expect the host and port number as parameters
	if (argc != 2) {
		std::cerr << "Usage: pdp <configfile>\n";
		return 1;
	}

	try {
		// read the configuration file for the naavpn
		std::string filename = argv[1];
		tncfhh::Configuration* config = tncfhh::Configuration::instance();
		config->addConfigurationFile(filename);

		// Initialize the NAL module, including the TLS socket
		boost::asio::io_service io_service;
		tnc::TNCServer s(io_service);


		// Starting the TNC server (IEL service)
		tncfhh::iel::coordinator.initialize();

		// Starting the NAL service
		s.accept();
		io_service.run();

	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}



