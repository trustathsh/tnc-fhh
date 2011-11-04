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
 
#ifndef VERSION_RESPONSE_MESSAGE_H_
#define VERSION_RESPONSE_MESSAGE_H_


/*
 * More information regarding the structure of the packets is
 * available in the official specification of IF-T binding to TLS.
 */

namespace ifttls {

/**
 * This class represents the version response message. This message is send as
 * a reply to a version request message. Belongs to the pre negotiation phase.
 *
 *
 *                       1                   2                   3
 *	 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *	|                 Reserved                      |    Version    |
 *	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 */
class VersionResponseMessage {
private:
	char reserved[3];

public:
	uint8_t version;


	/**
	 * Create a version_response_message with default version of 1
	 */
	VersionResponseMessage() : version(1)
	{
		memset(reserved, 0, 3);
	}


	/**
	 * Create a version_response_message with a certain version number
	 */
	VersionResponseMessage(uint8_t version) : version(version)
	{
		memset(reserved, 0, 3);
	}


	/**
	 * Creates a version_response_message on the basis of a given payload
	 * which should be of length 4
	 */
	VersionResponseMessage(char* msg)
	{
		memcpy(&reserved, msg, 3);
		memcpy(&version, msg+3, 1);
	}


	~VersionResponseMessage()
	{
		/* nothing */
	}


	/**
	 * Returns the payload as 4 byte long buffer, the buffer must be freed
	 * by the caller
	 */
	char* getMessage() {
		char *msg = new char[4];
		memcpy(msg, reserved, 3);
		memcpy(msg+3, &version, 1);
		return msg;
	}


	/**
	 * Returns the length of the message.
	 */
	uint32_t getLength() {
		return 4;
	}

	uint8_t getVersion() const { return version; }
};

} //namespace ifttls

#endif
