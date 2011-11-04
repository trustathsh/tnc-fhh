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
 
#ifndef ENDPOINT_DB_H
#define ENDPOINT_DB_H

#include <iostream>
#include <string>

#include <boost/asio.hpp>

#include <tr1/unordered_map>
using std::tr1::hash;

#include <tncvpn/iel/Endpoint.h>


namespace tnc {


typedef std::tr1::unordered_map<unsigned long, Endpoint*> EP_HashMap;


/**
 * Contains a list of all endpoints. The database is realized as
 * a hashmap.
 */
class EndpointDB {
private:
	/** Hashmap containing the endpoints */
	EP_HashMap endpoints;

public:
	/**
	 * Contsructs a new endpoint_db object.
	 */
	EndpointDB();
	virtual ~EndpointDB();

	/**
	 * Adds an endpoint to the db.
	 */
	void add(Endpoint *ep);


	/**
	 * Removes an endpoint to the db.
	 */
	void remove(Endpoint *ep);


	/**
	 * Returns a hashmap of all endpoints.
	 */
	EP_HashMap* getEndpoints();


	/**
	 * Prints all endpoints on stdout.
	 */
	void printAllEndpoints() const;
};

}

#endif
