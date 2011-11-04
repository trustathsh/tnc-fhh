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
 
#include "TNCClient.h"
#include <tncutil/Configuration.h>

/**
 * Starts the demo prototype of the TNC related part of the
 * Access Requestor.
 */
int main(int argc, char* argv[]) {
	try {
		// We expect the host and port number as parameters
		if (argc != 2) {
			std::cerr << "Usage: " << argv[0] << " <config>\n";
			return 1;
		}

		// read the configuration file for the naavpn
		std::string filename = argv[1];
		tncfhh::Configuration* config = tncfhh::Configuration::instance();
		config->addConfigurationFile(filename);
		config->get("VERIFY_FILE");

		// Initialize the boost networking related parts
		boost::asio::io_service io_service;
		boost::asio::ip::tcp::resolver resolver(io_service);
		boost::asio::ip::tcp::resolver::query query(
				config->get("PDP_ADDR"),
				config->get("PDP_PORT"));


		boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);
		boost::asio::ssl::context ctx(io_service, boost::asio::ssl::context::sslv23);
		ctx.set_verify_mode(boost::asio::ssl::context::verify_peer);
		ctx.load_verify_file(config->get("VERIFY_FILE"));

		// Create the TNC client
		tnc::TNCClient c(io_service, ctx, iterator);
		// start the tnc client
		c.doOperations();

	} catch (...) {
		std::cerr << "Exception.....: " << std::endl;
	}

	std::cerr << "Huh, is it over? Can I go now? Please..." << std::endl;

	return 0;
}
