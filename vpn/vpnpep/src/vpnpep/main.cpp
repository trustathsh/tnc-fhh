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
 
#include <iostream>
#include <string>

#include <boost/thread.hpp>

#include <tncutil/Configuration.h>
#include "PEPServer.h"

/**
 * The PEP daemon main method
 */
int main(int argc, char **argv) {
	// We expect the host and port number as parameters
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <configfile>\n";
		return 1;
	}

	std::cout << "[PEP daemon] Awaiting IF-PEP messages" << std::endl;

	tnc::EndpointDB db;

	tnc::AssessmentScheduler scheduler(&db);
	boost::thread* scheduler_thread = 0;

	// Creating and starting the assessment scheduler in an own thread
	scheduler_thread = new boost::thread(scheduler);

	try {
		// read the configuration file for the naavpn
		std::string filename = argv[1];
		tncfhh::Configuration* config = tncfhh::Configuration::instance();
		config->addConfigurationFile(filename);

		boost::asio::io_service io_service;
		tnc::PEPServer s(io_service, &db);
		io_service.run();

		// Wait for the thread to finish
		scheduler_thread->join();
	}
	catch (std::exception& e) {
		std::cerr << "[PEP daemon] Exception: " << e.what() << "\n";
	}


	return 0;
}
