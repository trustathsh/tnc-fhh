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
 
/*
 * TNCClient.cpp
 *
 *  Created on: 25.02.2010
 *      Author: tnc
 */

#include "TNCClient.h"

#define LOGNAME "[TNCClient]"
#include <log4cxx/logger.h>
static log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger(LOGNAME);

using namespace boost::asio;

namespace tnc {


TNCClient::TNCClient(io_service& io_service,
		boost::asio::ssl::context& context,
		ip::tcp::resolver::iterator endpoint_iterator) :
		tnc::ssl::Client(io_service,
		context, endpoint_iterator),
		pre_negotiation_finished(false),
		// we expect the socket to be constructed here
		ifttlsConnection(socket_)
{
	LOG4CXX_TRACE(logger, "Constructor");
}

TNCClient::~TNCClient()
{
	LOG4CXX_TRACE(logger, "Destructor");
}

void TNCClient::doOperations()
{
	LOG4CXX_TRACE(logger, "Let ifttlsConnection make version negotiation...");
	ifttlsConnection.startVersionNegotiation();

	LOG4CXX_TRACE(logger, "Let libTncClient initialize...");
	libTncClient.beginSession();
	LOG4CXX_INFO(logger, "Begin Handshake Loop");

	while(!libTncClient.isFinished()) {
		ifttls::Envelope *env = NULL;
		if (libTncClient.wantWrite()) {
			LOG4CXX_TRACE(logger, "libTncClient has data to send.");
			env = libTncClient.getMessage();
			ifttlsConnection.sendMessage(env);
		} else if (libTncClient.wantRead()) {
			LOG4CXX_TRACE(logger, "libTncClient wants some data from us.");
			env = ifttlsConnection.receiveMessage();
			libTncClient.putData(env);
		}
		delete env;
	}
	LOG4CXX_INFO(logger, "Finished Handshake Outcome ???");
}

} //namespace tnc

