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
 * TNCClient.h
 *
 *  Created on: 25.02.2010
 *      Author: tnc
 */

#ifndef TNCCLIENT_H_
#define TNCCLIENT_H_

#include "tncvpn/ssl/Client.h"
#include "tncvpn/ifttls/IFTMessages.h"

#include "LibTNCClient.h"
#include "tncvpn/ifttls/IFTTLSConn.h"

namespace tnc {

class TNCClient : public tnc::ssl::Client {

	LibTNCClient libTncClient;
	ifttls::IFTTLSConnection ifttlsConnection;

	/** Indicates whether the pre nogotiation phase has finished */
	bool pre_negotiation_finished;

	char header[ifttls::Envelope::IFT_HEADER_LENGTH];
	char *curMsg_;
	uint32_t curMsgLength_;


public:

	/**
	 * Just initializes the tnc_client, does start the TLS handshake.
	 */
	TNCClient(boost::asio::io_service& io_service,
			boost::asio::ssl::context& context,
			boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

	~TNCClient();

	/**
	 * Checks whether the TNC client wants to send or receive any data.
	 * The corresponding read and write operations are implemented
	 * asynchronously.
	 */
	void doOperations();
};

};

#endif /* TNCCLIENT_H_ */
