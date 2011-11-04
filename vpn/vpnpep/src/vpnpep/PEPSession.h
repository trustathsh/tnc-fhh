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

#include "tncvpn/ssl/Session.h"
#include "tncvpn/libtnac/AllowDenyPacket.h"
#include "tncvpn/libtnac/TLVBuffer.h"


namespace tnc {

/**
 * The pep_session class represents a TLS session between the
 * PEP daemon and the Policy Decision Point. It extends the ssl_session
 * object to inherit commonly used functions.
 */
class PEPSession : public ssl::Session {
private:
	/**
	 * Each recommendation results in an entry in
	 * the endpoint_db.
	 */
	tnc::EndpointDB *db;

public:
	/**
	 * Creates a pep_session object and initializes the session.
	 */
	PEPSession (boost::asio::io_service& io_service, boost::asio::ssl::context& context,
				tnc::EndpointDB *db)
	    : ssl::Session(io_service, context), db(db)
	{
	}


	void handleClient() {

    	std::cout << "[PEP daemon] TLS tunnel established!" << std::endl;
	std::cout << "[PEP daemon] Awaiting IF-PEP message..." << std::endl;

	// We expect any data from the PDP
    	socket_.async_read_some(boost::asio::buffer(data_, max_length),
    			boost::bind(&PEPSession::handleReadCompleted, this,
    			boost::asio::placeholders::error,
    			boost::asio::placeholders::bytes_transferred));
	}


	/**
	 * This method is invoked as far as data were received.
	 */
	void handleReadCompleted(const boost::system::error_code& error, size_t bytes_transferred) {
		if (!error) {
			// Read a simplified IF-PEP packet and print
			// the contents on stdout
			pep::AllowDenyPacket pkt(data_);
			pkt.printMessage();

			char recommendation = pkt.getRecommendation();
			char* ip = pkt.getIp();
			Endpoint *ep = new Endpoint(ip);
			db->add(ep);

			enforceBinaryIso(ip, recommendation);

		}
	    else {
	        delete this;
	    }
	}

};

}
