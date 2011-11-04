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
 
#ifndef IFT_ENVELOPE_H_
#define IFT_ENVELOPE_H_


/*
 * More information regarding the structure of the packets is
 * available in the official specification of IF-T binding to TLS.
 */

#include <string>
#include <iostream>

#include <boost/asio/ssl.hpp>
#include <boost/thread.hpp>

namespace ifttls {

/**
 * This represents the IF-T message envelop
 *
 *                       1                   2                   3
 *	 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *	|    Reserved   |           Message Type Vendor ID              |
 *	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *	|                          Message Type                         |
 *	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *	|                         Message Length                        |
 *	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *	|                       Message Identifier                      |
 *	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *	|           Message Value (e.g. IF-TNCCS Message) . . . .       |
 *	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *
 */
class Envelope {

private:
	uint8_t  reserved;
	uint32_t message_type;
	uint32_t message_length;
	uint32_t message_identifier;
	char*    message_value;


protected:
	static const int header_len = 16;
	static char message_type_vendor_id[];

public:
	static const uint32_t IFT_HEADER_LENGTH = 0x10;

	static const uint32_t IFT_HEADER_LENGTH_OFFSET = 0x08;
	
	/** Data Transport */
	static const uint32_t IFT_TYPE_EXPERIMENT = 0;
	/** IF-T Pre-Negotiation */
	static const uint32_t IFT_VERSION_REQUEST = 1;
	/** IF-T Pre-Negotiation */
	static const uint32_t  IFT_VERSION_RESPONSE = 2;
	/** IF-T Pre-Negotiation */
	static const uint32_t  IFT_CLIENT_AUTH_REQUEST = 3;
	/** IF-T Pre-Negotiation */
	static const uint32_t IFT_CLIENT_AUTH_SELECTION = 4;
	/** IF-T Pre-Negotiation */
	static const uint32_t IFT_CLIENT_AUTH_CHALLENGE = 5;
	/** IF-T Pre-Negotiation */
	static const uint32_t IFT_CLIENT_AUTH_RESPONSE = 6;
	/** IF-T Pre-Negotiation */
	static const uint32_t IFT_CLIENT_AUTH_SUCCESS = 7;
	/** Data Transport */
	static const uint32_t IFT_TNCCS_20_BATCH = 8;


	/**
	 * Constructs an empty IF-T message
	 */
	Envelope();


	/**
	 * Constructs an IF-T message on the basis of a given
	 * and length.
	 */
	Envelope(const char* message, uint32_t len);


	/**
	 * Constructs an IF-T message on the basis of a message type, payload and
	 * the corresponding length.
	 */
	Envelope(uint32_t type, const char* message_value, uint32_t len);
	virtual ~Envelope();


	/**
	 * Returns the message type
	 */
	uint32_t getMessageType() const;


	/**
	 * Returns the message length
	 */
	uint32_t getMessageLength() const;


	/**
	 * Returns the message as buffer, the returned buffer must be freed
	 * by the caller. The values for identifier, version etc. are in network
	 * byte order.
	 */
	char* getMessage();


	/**
	 * Returns the length of the message value (payload)
	 */
	uint32_t getMessageValueLength() const;


	/**
	 * Returns the message value (payload) as buffer, the returned buffer
	 * must be freed by the caller
	 */
	char* getMessageValue();

	uint32_t getIdentifier() const
	{
		return message_identifier;
	}

	void setIdentifier(uint32_t identifier) {
		message_identifier = identifier;
	}


	/**
	 * Prints the messages on stdout
	 */
	void printMessage();
};

}

#endif
