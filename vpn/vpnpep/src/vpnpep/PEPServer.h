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

#include "EndpointDB.h"
#include "AssessmentScheduler.h"
#include "EnforcementManager.h"
#include "PEPSession.h"

#include <tncvpn/ssl/Server.h>
#include <tncvpn/libtnac/AllowDenyPacket.h>
#include <tncvpn/libtnac/TLVBuffer.h>


namespace tnc {

/** PEP daemon listen on port 12346 */
const short TNC_PEPD_PORT = 12346;

/**
 * The pep_server listens for incoming IF-PEP messages.
 */
class PEPServer: public ssl::Server {

private:
	/** The endpoint_db is passed to the session objects */
	tnc::EndpointDB *db;

public:

	/**
	 * Creates the server and awaits the first connection.
	 */
	PEPServer(boost::asio::io_service& io_service, tnc::EndpointDB *db) :
		ssl::Server(io_service, TNC_PEPD_PORT), db(db) {

		accept();
	}


	/**
	 * Accept a connection and create a corresponding pep_session object.
	 */
	void accept() {
		PEPSession* new_session = new PEPSession(io_service_, context_, db);

		// "Wait" for the first incoming client request
		acceptor_.async_accept(new_session->socket(), boost::bind(
				&PEPServer::handleAcceptCompleted, this, new_session,
				boost::asio::placeholders::error));

	}


	/**
	 * This method is invoked as far as a connection between PEP and PDP has
	 * been established. Creates a pep_session object for the connection and
	 * executes an accept call in order to handle further PDP recommendations.
	 */
	void handleAcceptCompleted(PEPSession* new_session,
			const boost::system::error_code& error) {
		if (!error) {

			// Handle the client
			new_session->doHandshake();
			std::cout << "[PEP daemon] Recommendation received " << new_session << std::endl;
			new_session = new PEPSession(io_service_, context_, db);


			// Await the next connection
			acceptor_.async_accept(new_session->socket(), boost::bind(
					&PEPServer::handleAcceptCompleted, this, new_session,
					boost::asio::placeholders::error));
		} else {
			delete new_session;
		}
	}

};

}
