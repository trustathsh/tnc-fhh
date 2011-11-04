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
 
#include "TNCSession.h"
#include <arpa/inet.h>

#include <log4cxx/logger.h>

#define LOGNAME "[TNCSession]"
static log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger(LOGNAME);

namespace tnc {

TNCSession::TNCSession (boost::asio::io_service& io_service, boost::asio::ssl::context& context)
	: tnc::ssl::Session(io_service, context), 
	handler(0), 
	ep(0),
	ifttlsConnection(0)
{

}

TNCSession::~TNCSession()
{
	LOG4CXX_TRACE(logger, "Destructor");
	delete handler;
	delete ep;
	delete ifttlsConnection;
}

void TNCSession::handleClient() {
    	// Create an Endpoint object on the basis of the IP address
    	boost::asio::ip::tcp::endpoint remote_ep = socket_.lowest_layer().remote_endpoint();
	boost::asio::ip::address remote_ad = remote_ep.address();
	ep = new tnc::Endpoint(remote_ad);

	LOG4CXX_INFO(logger, "New Request from endpoint: " << remote_ad);

	// Creating the message_handler
	handler = new MessageHandler(ep);

	ifttlsConnection = new ifttls::IFTTLSConnection(socket_);

	// Start with the operations
	doOperations();
}


void TNCSession::doOperations() {
	try {
		LOG4CXX_TRACE(logger, "Expecting Version Negotiation");
		ifttlsConnection->expectVersionNegotiation();


		while (!handler->isFinished()) {
			ifttls::Envelope *env;
			// Does the TNC server want to read?
			if (handler->wantRead()) {
				LOG4CXX_TRACE(logger, "handler wants some data");
				env = ifttlsConnection->receiveMessage();
				handler->handleMessage(env);

			} else if (handler->wantWrite()) {
				LOG4CXX_TRACE(logger, "handler wants to send data");

				// Are there any messages to send?
				if (handler->hasReplyMessage()) {
					env = handler->getReplyMessage();
					ifttlsConnection->sendMessage(env);
				}
			}
			delete env;
			handler->done();
		}
		// send the last message, which should be the recommendation
		if (handler->hasReplyMessage()) {
			ifttls::Envelope *reply = handler->getReplyMessage();
			ifttlsConnection->sendMessage(reply);
			delete reply;
		}
		LOG4CXX_INFO(logger, "Session Over");
		// TODO: could be more specific here
	} catch (...) {
		LOG4CXX_ERROR(logger, "Exception");
		// just do nothing for now... :-/
	}
}
};
