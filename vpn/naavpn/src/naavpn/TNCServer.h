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
 
#ifndef TNCSERVER_H
#define TNCSERVER_H

#include "tncvpn/ssl/Server.h"
#include "TNCSession.h"

namespace tnc {

/**
 * The tnc_server extends the ssl_server object in order to
 * inherit also the TLS related functions. Accepts TLS connections
 * from the Access Requestors
 */
class TNCServer: public tnc::ssl::Server {

public:

	/**
	 * Initializes the tnc_server object and awaits endpoint
	 * connections.
	 */
	TNCServer(boost::asio::io_service& io_service);

	/**
	 * Await a connection attempt.
	 */
	void accept();

	/**
	 * This method is invoked as far as a client connection has been established.
	 * Creates a tnc_session object for that connection and performs an accept call
	 * in order to handle further client requests.
	 */
	void handleAccept(TNCSession* new_session, const boost::system::error_code& error);

};

}

#endif

