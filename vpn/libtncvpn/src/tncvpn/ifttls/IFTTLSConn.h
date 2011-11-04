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
 * IFTTLSConn.h
 *
 *  Created on: Aug 1, 2010
 *      Author: awelzel
 */

#ifndef IFTTLSCONN_H_
#define IFTTLSCONN_H_

#include "tncvpn/ifttls/IFTMessages.h"

#include <boost/asio.hpp>

/* forgive me */
using namespace boost::asio;

namespace ifttls {

/*
 * Encapsulate some of the functionality to send an receive if-t-ttls messages
 * in this class. A socket which is ready to read/write operations is expected.
 * TLS handshake needs to be finished before the a IFTTLSConnection object
 * may be created.
 */
class IFTTLSConnection {

public:

	IFTTLSConnection(ssl::stream<ip::tcp::socket>& sock);
	~IFTTLSConnection();

	void startVersionNegotiation();
	void expectVersionNegotiation();

	void sendMessage(Envelope *envelope);
	ifttls::Envelope *receiveMessage();

private:
	// Our socket, copied from Client.h
	ssl::stream<ip::tcp::socket>& socket_;

	/*
	 * We first receive the header. Its length is always IFT_HEADER_LENGTH.
	 */
	uint8_t curHeader[Envelope::IFT_HEADER_LENGTH];

	/*
	 * Field where the current message length is stored.
	 */
	uint32_t curMsgLength;

	/*
	 * Pointer to the whole message, the header will be copied in here.
	 */
	uint8_t *curMessage;

	/*
	 * the current identifier to be used
	 */
	uint32_t curIdentifier;

	uint32_t getNextIdentifier() {
		uint32_t tmp = curIdentifier;
		curIdentifier++;
		return tmp;
	}

	void readHeader();
	void allocateMessageBuffer();
	void readRestOfMessage();
	Envelope *constructEnvelope();
	void freeBuffers();
};

}

#endif /* IFTTLSCONN_H_ */
