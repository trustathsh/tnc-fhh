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
 
#ifndef TNCSESSION_H
#define TNCSESSION_H

#include "tncvpn/ssl/Session.h"
#include "tncvpn/ifttls/Envelope.h"
#include "tncvpn/ifttls/IFTTLSConn.h"

#include "MessageHandler.h"

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

namespace tnc {

/**
 * The tnc_session class represents one TLS session between
 * Access Requestor and Policy Decision Point. This class handles
 * the IF-T binding to TLS message exchange and forwards the IF-T
 * messages to the message_handler.
 */

class TNCSession : public tnc::ssl::Session {
private:

	/* space where the header and message body gets stored */
	unsigned char header_[ifttls::Envelope::IFT_HEADER_LENGTH];
	unsigned char *curMsg_;
	uint32_t curMsgLength_;

	MessageHandler *handler;
	tnc::Endpoint *ep;
	ifttls::IFTTLSConnection *ifttlsConnection;

public:

	/**
	 * Default constructor, initializes the object with some default values.
	 */
	TNCSession (boost::asio::io_service& io_service, boost::asio::ssl::context& context);


	~TNCSession();

	/*
	 * Method called after TLS handshake is completed
	 */
	void handleClient();


	/**
	 * The do_operations method invokes the read and write operations
	 * asynchronous in order to realize that more than one read or write
	 * operation can be performed until the first one finishes.
	 */
	void doOperations();


	/**
	 * Callback gets invoked as far as we have received any data. Passes the IF-T
	 * to the TNC server via the message_handler.
	 *
	void handleReadCompleted(const boost::system::error_code& error, size_t bytes_transferred);
	*/


	/**
	 * Callback gets invoked as far as we have received any data. Passes the IF-T
	 * to the TNC server via the message_handler.
	 *
	void handleWriteCompleted(const boost::system::error_code& error);
	*/

	/*
	void handleReadHeader(const boost::system::error_code& error, size_t bytes_transferred);
	void handleReadBody(const boost::system::error_code& error, size_t bytes_transferred);
	*/
};

}

#endif


