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
 


#ifndef TLV_BUFFER_H
#define TLV_BUFFER_H


#include <stdint.h>
#include <iostream>
#include <cstring>

#include <exception>


using namespace std;


/**
 * Simplifies the handling of arbitrary TLV packets.
 * @author Alexander Reich
 */
template<typename T>
class TLVBuffer {


private:
	/** TLV offset in the packet header */
	unsigned offset;

	/** Current len of the buffer */
	unsigned buf_len;

	/** size of the buffer */
	unsigned buf_size;

	unsigned max_pkt_size;
	char* buffer;
	char* packet;

	/** New packet is available */
	bool packet_complete;

	/** bytes read of the packet */
	unsigned read_len;

	bool new_packet;

	/** Length of the packet */
	T packet_len;


public:
	TLVBuffer(unsigned offset, unsigned max_pkt_size, unsigned buf_size = 4096)
		: offset(offset), buf_len(0), max_pkt_size(max_pkt_size), buf_size(buf_size),
		  packet_complete(true), new_packet(false), read_len(0) {

		buffer = new char[2*buf_size];
		packet = new char[max_pkt_size];
	}


	~TLVBuffer() {
		delete[] buffer;
		delete[] packet;
		buffer = 0;
		packet = 0;
	}


	void addToBuffer(char* data, unsigned len) {
		if (buf_len + len > 2*buf_size) throw std::exception();

		memcpy(buffer+buf_len, data, len);
		buf_len += len;
		parse();
	}


	bool isPacketComplete() {
		// packet_complete just indicates whether a packet is in progress or not
		return packet_complete && buf_len > 0;
	}

	bool isNewPacket() {
		return new_packet;
	}

	void copyData(unsigned packet_len) {

		// buffer contains more than just one packet
		if (buf_len > packet_len) {
			// Copy the whole packet into the packet buffer
			memcpy(packet, buffer, packet_len);

			// Adapt the buffer
			buf_len -= packet_len;
			memcpy(buffer, buffer+packet_len, buf_len);

			packet_complete = true;
			new_packet = true;
		}
		// buffer contains exactly one packet
		else if (buf_len == packet_len) {
			memcpy(packet, buffer, packet_len);
			packet_complete = true;
			buf_len = buf_len - packet_len;
			new_packet = true;
		}
		// buffer contains less than one packet
		else {
			read_len = buf_len;
			packet_complete = false;
			new_packet = false;
		}

	}

	void nextPacket() {
		new_packet = false;

		if (packet_complete)
			read_len = 0;

		if (buf_len > 0)
			parse();

	}

	void parse() {
		memcpy(&packet_len, buffer+offset, sizeof(T));
		copyData(packet_len);
	}

	T getPacketLenght() {
		return packet_len;
	}

	char* getPacket() {
		return packet;
	}
};

#endif
