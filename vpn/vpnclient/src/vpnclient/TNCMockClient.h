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
 * TNCMockClient.h
 *
 *  Created on: 25.02.2010
 *      Author: tnc
 */

#ifndef TNCMOCKCLIENT_H_
#define TNCMOCKCLIENT_H_

#include <iostream>
#include <fstream>
#include <string>

#include <tncs/iel/Coordinator.h>


/**
 * Mock object which emulates a real TNC client. This
 * class replays already captured TNCCS data in order to
 * get access to the network.
 */
class TNCMockClient {


private:
	/**
	 * Indicates which message shall be send next.
	 */
	int msg_count;

	/** Indicates that the TNC client wants to read */
	bool _want_read;

	/** Indicates that the TNC client wants to write */
	bool _want_write;


protected:

	/**
	 * Reads an already captured TNCCS message from the file system
	 * and returns it as a std::string.
	 */
	std::string readFile(const char* file_name) {
		char line[1024];
		std::ifstream file;
		std::string file_str;

		file.open(file_name, std::ios::in);

		if (file.good()) {
			file.seekg(0L, std::ios::beg);

			while (!file.eof()) {
				file.getline(line, 1024);
				file_str.append(line);
			}
		}
		else {
			std::cout << "File not found!" << std::endl;
		}

		return file_str;
	}

public:
	/**
	 * Create a tncc object and initializes it.
	 */
	TNCMockClient() : msg_count(0), _want_read(false), _want_write(true) {}

	virtual ~TNCMockClient() {}


	/**
	 * Returns an already a TNCCS message which shall be
	 * send to the Policy Decision Point.
	 */
	tncfhh::iel::TNCCSData getMessage() {
		std::string snip;
		if (msg_count == 0) {
			snip = readFile("snip-1.xml");
			_want_read = true;
			_want_write = false;
		}
		else if (msg_count == 1) {
			snip = readFile("snip-3.xml");
			_want_read = false;
			_want_write = false;
		}


		TNC_BufferReference buf = (unsigned char*) snip.c_str();
		tncfhh::iel::TNCCSData data(buf, snip.length());
		return data;
	}

	/**
	 * Since this TNC client is just a dummy implementation, this
	 * method only increments the message counter.
	 */
	void putMessage(tncfhh::iel::TNCCSData data) {
		std::cout << "count++\n";
		msg_count++;

		_want_read = false;
		_want_write = true;
	}

	/**
	 * Indicates that the TNC client wants to read.
	 */
	bool wantRead() {
		return _want_read;
	}

	/**
	 * Indicates that the TNC client wants to write.
	 */
	bool wantWrite() {
		return _want_write;
	}

};

#endif /* TNCMOCKCLIENT_H_ */
