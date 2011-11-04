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
 

#ifndef ALLOW_DENY_PACKET_H
#define ALLOW_DENY_PACKET_H


#include <string>
#include <boost/asio.hpp>
#include <stdint.h>


namespace pep {


/**
 * The only packet type of the simplified IF-PEP
 * protocol. Supports only binary-based isolation.
 */
class AllowDenyPacket {
private:

	/* The ip address of the Access Requestor */
	char* ip;

	/* The recommendation, either 'Y' or 'N' */
	char recommendation;

	/** The packet buffer */
	char* packet;

	/** Used for some packetz manipulation operations */
	char* pck_ptr;

	/** Size of the packet */
	unsigned int pck_size;


protected:
	/**
	 * Adds an element to packet.
	 */
	char* addElement(const void*, int);


public:
	/**
	 * Creates a new packet on the basis of a buffer.
	 */
	AllowDenyPacket(const char* packet);


	/**
	 * Creates a new packet on the basis of the IP address
	 * of an Access Requestor and a recommendation flag.
	 */
	AllowDenyPacket(boost::asio::ip::address*, char);

	virtual ~AllowDenyPacket();


	/**
	 * Returns the packet as a pointer to a buffer.
	 */
	char* getPacket();


	/**
	 * Returns the size of the packet.
	 */
	unsigned int getPacketSize() const;


	/**
	 * Prints the packet information on stdout.
	 */
	void printMessage() const;


	/**
	 * Returns the IP address used within the packet.
	 */
	char* getIp() const;


	/**
	 * Returns the recommendation
	 */
	char getRecommendation() const;
};

}
#endif
