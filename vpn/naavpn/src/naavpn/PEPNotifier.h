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
 
#ifndef PEPNOTIFIER_H
#define PEPNOTIFIER_H

#include <tncvpn/libtnac/AllowDenyPacket.h>
#include <tncvpn/ssl/Client.h>
#include <tncvpn/iel/Endpoint.h>

namespace tnc {

/**
 * The pep_notifier is responsible for sending a recommendation
 * to the Policy Enforcement Point. At the moment there is only
 * binay-based isolation and the simplified IF-PEP version supproted.
 */
class PEPNotifier {
protected:

	/**
	 * Builds the packet and sends the message
	 */
	void sendBinaryRecommendation(Endpoint* ep, char recommendation);


	/**
	 * Splits an IPv4 address into 4 octets and stores them in
	 * the 4 byte long buffer oct.
	 */
	void splitIpv4(std::string ip, char *oct);

	/**
	 * Prints an IPv4 address on the basis of the octets
	 */
	void printAddress(char oct[]) {
		std::printf("%d %d %d %d\n", (unsigned char)oct[0], (unsigned char)oct[1], (unsigned char)oct[2], (unsigned char)oct[3]);
	}

public:

	PEPNotifier() {}
	virtual ~PEPNotifier() {}


	void sendAllow(Endpoint* ep);
	void sendDeny(Endpoint* ep);



	/**
	 * Determines the responsible PEP for a certain Endpoint on the basis of
	 * the Endpoints IPv4 address and the defined mappings. These mappings are
	 * retieved by the configuration service (abstract from the configuration
	 * source like file or LDAP).
	 *
	 * IPv4 only at the moment!
	 *
	 * @return The IPv4 address of the PEP or 0.0.0.0 if no corresponding
	 * mapping was found.
	 */
	std::string getPEPForEndpoint(boost::asio::ip::address *ip);
};

}

#endif
