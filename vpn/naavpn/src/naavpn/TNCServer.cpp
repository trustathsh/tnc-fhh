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
 
#include "TNCServer.h"
#include <tncutil/Configuration.h>


#define LOGNAME "[TNCServer]"
#include <log4cxx/logger.h>
static log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger(LOGNAME);

namespace tnc {

const short tncServerPort() {
	short i;
	std::istringstream iss(tncfhh::Configuration::instance()->get("PDP_PORT"));
	iss >> i;
	return i;
}
	
TNCServer::TNCServer(boost::asio::io_service& io_service) :
	tnc::ssl::Server(io_service, tncServerPort()) {

	//accept();
}
	
void TNCServer::accept()  {
	TNCSession* new_session = new TNCSession(io_service_, context_);
	// "Wait" for the first incoming client request
	acceptor_.async_accept(new_session->socket(), boost::bind(
			&TNCServer::handleAccept, this, new_session,
			boost::asio::placeholders::error));
}

void TNCServer::handleAccept(TNCSession* new_session, const boost::system::error_code& error) {
	if (!error) {
		// create a new session to accept
		TNCSession* another_session = new TNCSession(io_service_, context_);
		// this calls immediately returns
		acceptor_.async_accept(another_session->socket(), boost::bind(
				&TNCServer::handleAccept, this, another_session,
				boost::asio::placeholders::error));

		// Handle the client connection and create a corresponding
		// session object for the endpoint.
		LOG4CXX_INFO(logger, "New Client Connection");
		new_session->doHandshake();
	}
	LOG4CXX_INFO(logger, "Remove Client Connection");
	delete new_session;
}

}
