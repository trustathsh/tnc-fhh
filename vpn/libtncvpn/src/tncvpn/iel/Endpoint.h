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
 
#ifndef ENDPOINT_H
#define ENDPOINT_H

#include <string>
#include <ctime>

#include <boost/asio.hpp>


namespace tnc {


/**
 * Represents an Access Requestor identified by an IP address.
 * Stores an additional attribute which indicates the time of the
 * last assessment.
 */
class Endpoint {
	boost::asio::ip::address address;
	time_t last_assessment;

public:
	/**
	 * Creates an new Endpoint based on the IP address.
	 */
	Endpoint(boost::asio::ip::address);

	/**
	 * Creates an new Endpoint based on the IP address.
	 */
	Endpoint(const char*);
	virtual ~Endpoint();


	/**
	 * Returns the IP address of an Endpoint.
	 */
	boost::asio::ip::address* getAddress();


	/**
	 * Returns the timestamp of the last assessment.
	 */
	const time_t* getLastAssessment() const;
};

}

#endif
