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
 
#ifndef PEPCLIENT_H
#define PEPCLIENT_H

#include "tncvpn/ssl/Client.h"
#include "tncvpn/libtnac/AllowDenyPacket.h"

#include <iostream>

namespace tnc {

/**
 * The pep_client class is responsible for sending an IF-PEP
 * message to the Policy Enforcement Point. The class extends
 * the tnc::ssl::Client base class since it sends the messages through
 * a TLS encrypted tunnel.
 */
class PEPClient : public tnc::ssl::Client {

private:
	/** Recommendation flag
	 * 		N: No access will be granted
	 * 		Y: Access will be granted
	 */
	char recommendation;

	/** Access Requestor IP address */
	boost::asio::ip::address *address;


public:

	/**
	 * Default constructor, just initializes the pep_client class
	 */
	PEPClient(boost::asio::io_service& io_service,
			boost::asio::ssl::context& context,
			boost::asio::ip::tcp::resolver::iterator endpoint_iterator,
			boost::asio::ip::address *address,
			char recommendation);

	/**
	 * Creates an IF-PEP message and sends it to the Policy Enforcement
	 * Point. The contents of this methods will be adapted in the future
	 * in order to support IF-PEP binding to RADIUS.
	 *
	 * This method gets invoked as far as the SSL handshake successfully returns.
	 */
	void handleHandshakeCompleted();

	/**
	 * Indicates that the IF-PEP message was sent to the PEP. Just used to print a
	 * log message to on stdout.
	 */
	void handleWrite(const boost::system::error_code& error, size_t bytes_transferred);

};

}

#endif
