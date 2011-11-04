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
 

#include "AllowDenyPacket.h"

#include <iostream>
#include <cstring>
#include <cstdlib>

#include <sys/socket.h>

namespace pep {

/*
 * See the header file for a description of the methods.
 */

const char* packet_identifier = "PEP";
const char ip_version_identifier = 4;


AllowDenyPacket::AllowDenyPacket(const char* packet)
 : ip(0), packet(0), pck_size(0) {

	// Copies the version identifier
	char *identifier;
	memcpy(&identifier, packet, 3);


	// Copies the IP version identifier
	char ip_vers;
	memcpy(&ip_vers, packet+3, 1);


	// Copies the recommendation to the packet
	memcpy(&recommendation, packet+3+1, 1);


	// Copies the IP address as 4 byte value to the packet
	int ip_bytes;
	memcpy(&ip_bytes, packet+3+1+1, 4);


	struct in_addr addr;
	addr.s_addr = ip_bytes;
	std::string tmp(inet_ntoa(addr));
	ip = new char[tmp.length()];
	strcpy(ip, tmp.c_str());


	/*
	uint32_t len = 2;
	memcpy(&len, packet, 4);

	// Copies the IP address as 4 byte value to the packet
	ip = new char(len+1);
	bzero(ip, len+1);
	memcpy(ip, packet+4, len);

	// Copies the recommendation to the packet
	memcpy(&recommendation, packet+4+len, 1);
*/
}


AllowDenyPacket::AllowDenyPacket(boost::asio::ip::address *address, char recommendation)
 : ip(0), packet(0), pck_size(0) {

	packet = new char(32);
	pck_ptr = packet;

	// Copies the IP address to the packet
	addElement(&packet_identifier, 3);


	// Copies the version_identifier to the packet
	addElement(&ip_version_identifier, 1);


	// Copies the recommendation to the packet
	addElement(&recommendation, 1);


	// Copies the IP address value
	boost::asio::ip::address_v4 v4 = address->to_v4();
	boost::array<unsigned char,4>  bytes = v4.to_bytes();
	char ip_bytes[4];
	ip_bytes[0] = bytes[0];
	ip_bytes[1] = bytes[1];
	ip_bytes[2] = bytes[2];
	ip_bytes[3] = bytes[3];
	addElement(ip_bytes, 4);


/*
	packet = new char(32);
	pck_ptr = packet;

	// Copies the IP address to the packet
	std::string ip = address->to_string();
	uint32_t len = ip.length();

	add_element(&len, 4);
	add_element(ip.c_str(), len);

	// Copies the recommendation to the packet
	add_element(&recommendation, 1);

	*/
}


AllowDenyPacket::~AllowDenyPacket() {
	if (packet != 0)
		delete packet;

	if (ip != 0)
		delete ip;
}

char*
AllowDenyPacket::getPacket() {
	return packet;
}


char*
AllowDenyPacket::addElement(const void* element, int size) {
	memcpy(pck_ptr, element, size);
	pck_ptr += size;
	pck_size += size;
}

unsigned int
AllowDenyPacket::getPacketSize() const {
	return pck_size;
}


void
AllowDenyPacket::printMessage() const {
	if (ip == 0) return;
	std::cout << "[IF-PEP] Recommendation for IP " << ip << " is " << recommendation << std::endl;
}


char*
AllowDenyPacket::getIp() const {
	return ip;
}

char
AllowDenyPacket::getRecommendation() const {
	return recommendation;
}


}
