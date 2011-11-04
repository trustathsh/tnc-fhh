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
 
#include "PEPClient.h"
#include "tncvpn/iel/Endpoint.h"

namespace tnc {

PEPClient::PEPClient(boost::asio::io_service& io_service,
		boost::asio::ssl::context& context,
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator,
		boost::asio::ip::address *address,
		char recommendation)
			: tnc::ssl::Client(io_service, context, endpoint_iterator),
			  recommendation(recommendation), address(address) {
	handleHandshakeCompleted();

}

void PEPClient::handleHandshakeCompleted() {
	std::cout << "[PEP-CLIENT] Connected to PEPd" << std::endl;

	// Create a packet of the simplified IF-PEP message.
	pep::AllowDenyPacket *pkt = new pep::AllowDenyPacket(address, recommendation);

	// Sends the packet to the PEP asynchronously
	boost::asio::async_write(socket_, boost::asio::buffer(
				 pkt->getPacket(), pkt->getPacketSize()),
				 boost::bind(&PEPClient::handleWrite, this,
						 boost::asio::placeholders::error,
						 boost::asio::placeholders::bytes_transferred));
}

void PEPClient::handleWrite(const boost::system::error_code& error, size_t bytes_transferred) {
	if (!error) {
			std::cout << "[PEP-CLIENT] Recommendation sent" << std::endl;
	} else {
		std::cout << "[PEP-CLIENT] Write failed: " << error << "\n";
	}
}

}
