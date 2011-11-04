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
 
#include "HostScannerIMC.h"
#include <errno.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

// include log4cxx header files.
#include "log4cxx/logger.h"
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/helpers/exception.h"
using namespace log4cxx;
using namespace log4cxx::helpers;

static LoggerPtr logger(Logger::getLogger("IMUnit.AbstractIMUnit.AbstractIMC.HostScannerIMC"));

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
HostScannerIMC::HostScannerIMC(TNC_ConnectionID cid, HostScannerIMCLibrary *pHostScannerIMCLibrary):
	tncfhh::iml::AbstractIMC(cid, pHostScannerIMCLibrary)
{
	//Easter
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
HostScannerIMC::~HostScannerIMC()
{
	//Egg
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result HostScannerIMC::beginHandshake()
{
    TNC_Result ret = TNC_RESULT_SUCCESS;

    LOG4CXX_DEBUG(logger, "Send Message: " << "HostScannerIMC active");
    tncc.sendMessage((unsigned char*)("HostScannerIMC active"), 22-1, TNC_MESSAGETYPE_FHH_HOSTSCANNER);

    return ret;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result HostScannerIMC::receiveMessage(TNC_BufferReference message, TNC_UInt32 messageLength, TNC_MessageType messageType)
{
    LOG4CXX_DEBUG(logger, "Receive Message. Length = " << messageLength << " Bytes");

	std::stringstream ss;

	ss.write((const char *)message, messageLength);

	size_t buf_size = 1000;
	char buf[1000];

	std::vector<Policy> status;

    while (readLine(&ss, buf, buf_size)) {
    	std::string line(buf);

		/* read Protocol */
		Protocol protocol;
		if (!line.compare(0, 3, "TCP"))
			protocol = TCP;
		else if (!line.compare(0, 3, "UDP"))
			protocol = UDP;
		else {
		    LOG4CXX_WARN(logger, "Protocol is unknown (not TCP or UDP).");
		    continue;
		}

		/* read Port */
		size_t tmpPos = line.find_first_of("1234567890", 3);
		if (tmpPos==std::string::npos){
		    LOG4CXX_WARN(logger, "No port number found.");
		    continue;
		}

		char * end = NULL;
		unsigned long int port = strtoul(line.substr(tmpPos, line.size()).c_str(), &end, 10);
		if (errno==ERANGE || errno==EINVAL /* || *end */) // FIXME valgrind memory invalid read
		{
		    LOG4CXX_WARN(logger, "Error converting string to unsigned int.");
		    continue;
		}

		/* Test port */
		{
	        struct sockaddr_in addr;
	        struct servent *serv;
	        int s;
	        bool portOpen = false;

	        addr.sin_addr.s_addr = htonl(inet_addr("1.0.0.127"));
            s = socket(PF_INET, SOCK_STREAM/*(protocol==TCP?SOCK_STREAM:SOCK_DGRAM)*/, 0);
            if (s == -1)
            {
            	LOG4CXX_WARN(logger, "socket failed()!");
            	continue;
            }
            addr.sin_port = htons(port);
            addr.sin_family = AF_INET;


            portOpen = !connect(s, (struct sockaddr*)&addr, sizeof(addr));
            std::string portOpenString;

            if (portOpen) {
                ::close(s);
                status.push_back(Policy(protocol, port, open));
                portOpenString = "open";
            } else {
                status.push_back(Policy(protocol, port, close));
                portOpenString = "close";
            }

            serv = getservbyport(addr.sin_port, (protocol==TCP?"tcp":"udp"));
            if (serv) {
				LOG4CXX_DEBUG(logger, (protocol==TCP?"TCP":"UDP") << " Port " << port << "(" << serv->s_name << ") " << portOpenString);
//                log.debug("%s Port %i (%s) %s", (protocol==TCP?"TCP":"UDP"), port, serv->s_name, portOpenString.c_str());
            } else {
				LOG4CXX_DEBUG(logger, (protocol==TCP?"TCP":"UDP") << " Port " << port << " (unknown) " << portOpenString);
//                log.debug("%s Port %i (unknown) %s", (protocol==TCP?"TCP":"UDP"), port, portOpenString.c_str());
            }
        }
	}

	std::stringstream send;

	for (std::vector<Policy>::iterator iter = status.begin();
	    iter != status.end();
	    ++iter) {
		send << (iter->protocol==TCP?"TCP":"UDP") << " " << std::dec << iter->port << " = " << (iter->status==open?"open":"close") << std::endl;
	}

	std::string sendString = send.str();

	LOG4CXX_DEBUG(logger, "Send Message: Length = " << sendString.size() << " Bytes.");
	this->tncc.sendMessage((unsigned char*)sendString.c_str(), sendString.size(), TNC_MESSAGETYPE_FHH_HOSTSCANNER);

	return TNC_RESULT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
HostScannerIMC::Policy::Policy(Protocol pr, unsigned short po, Status s)
:protocol(pr), port(po), status(s)
{/*wee*/}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool HostScannerIMC::readLine(std::istream *in, char *buf, const int size) {
	if(in == NULL)
	{
		std::cerr << "Could not read file. std::istream == NULL !" << std::endl;
		return false;
	} // else
    bool ret = in->getline(buf, size);
    if (ret && buf[strlen(buf) - 1] == '\r') {
        buf[strlen(buf) - 1] = '\0';
    }
    return ret;
}
