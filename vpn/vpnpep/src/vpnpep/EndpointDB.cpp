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
 
#include "EndpointDB.h"


namespace tnc {

/*
 * See the header file for a description of the methods.
 */

EndpointDB::EndpointDB() {

}

EndpointDB::~EndpointDB() {

}


void
EndpointDB::add(Endpoint *ep) {
	boost::asio::ip::address* addr = ep->getAddress();

	unsigned long conn_id;

	// Only IPv4 is supported at the moment
	if (addr->is_v4()) {
		conn_id = addr->to_v4().to_ulong();
	} else if(addr->is_v6()) {
		std::cerr << "IPv6 isn't support!" << std::endl;
		return;
	}

	// Add the endpoint to the db
	endpoints[conn_id] = ep;
}


void
EndpointDB::remove(Endpoint *ep) {

}

EP_HashMap*
EndpointDB::getEndpoints() {
	return &endpoints;
}

void
EndpointDB::printAllEndpoints() const {
	for(EP_HashMap::const_iterator it = endpoints.begin(); it != endpoints.end(); ++it) {
//		struct tm* ts = localtime(it->second->getLastAssessmentTime());
//		printf("Item found for IP: %d\n", it->second->getIP()->getHostAddress());
//		printf("Item found: %d\n", it->first);

		std::cout << "[Endpoint DB] Endpoint: " << it->second->getAddress()->to_string() << std::endl;
	}
}


}
