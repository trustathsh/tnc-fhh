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
 
#include "Endpoint.h"

namespace tnc {

/*
 * See the header file for a description of the methods.
 */

Endpoint::Endpoint(boost::asio::ip::address address)
	: address(address) {

	// Initialize with current time
	last_assessment = time(NULL);
}


Endpoint::Endpoint(const char* ip) {

	// Create a boost address object by a char*
	address = boost::asio::ip::address::from_string(ip);

	// Initialize with current time
	last_assessment = time(NULL);
}

Endpoint::~Endpoint() {

}


boost::asio::ip::address*
Endpoint::getAddress() {
	return &address;
}

const time_t*
Endpoint::getLastAssessment() const {
	return &last_assessment;
}

}
