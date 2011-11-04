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
 
#include "ClamavIMC.h"

#include <log4cxx/logger.h>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm> //std::remove

#include <sys/types.h> // sockets
#include <sys/socket.h> // sockets
#include <sys/un.h> // sockets
#include <unistd.h> // close
#include <sys/wait.h> // wait

using namespace log4cxx;

static LoggerPtr logger(Logger::getLogger(
		"IMUnit.AbstractIMUnit.AbstractIMC.ClamavIMC"));

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
ClamavIMC::ClamavIMC(TNC_ConnectionID conID, ClamavIMCLibrary *pClamavIMCLibrary) :
	AbstractIMC(conID, pClamavIMCLibrary), clamconfPath(""),
	clamdSocketPath(""), resultOperationalStatus(ClamavIMCLibrary::UNKNOWN) {
	//
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
ClamavIMC::~ClamavIMC() {
	//
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result ClamavIMC::beginHandshake() {
	LOG4CXX_TRACE(logger, "beginHandshake");
	// measure
	this->measure();
	// send message to IMV
	this->tncc.sendMessage((unsigned char*) this->out.str().c_str(),
			this->out.str().size(), TNC_MESSAGETYPE_FHH_CLAMAV);
	return TNC_RESULT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result ClamavIMC::receiveMessage(TNC_BufferReference message,
		TNC_UInt32 messageLength, TNC_MessageType messageType) {
	LOG4CXX_TRACE(logger, "receiveMessage");
	// we should not receive a message from the IMV in this version
	// but since this IMC is in an early stage, we will redo the measurement
	// and sent out a new message to the IMV.
	// in a later version, remediation instructions must be handled here
	this->measure();
	this->tncc.sendMessage((unsigned char*) this->out.str().c_str(),
			this->out.str().size(), TNC_MESSAGETYPE_FHH_CLAMAV);
	return TNC_RESULT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result ClamavIMC::batchEnding() {
	LOG4CXX_TRACE(logger, "batchEnding");
	return TNC_RESULT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ClamavIMC::readConfiguration() {
	LOG4CXX_DEBUG(logger, "Reading configuration from " << IMC_CONFIG);
	std::ifstream file(IMC_CONFIG);
	std::string line;
	std::string key;
	std::string value;
	int posSpace;

	// read file
	while (std::getline(file, line)) {
		// skip empty lines
		if (line.length() == 0)
			continue;
		// skip comments
		if (line[0] == '#')
			continue;

		// find delimiting space
		posSpace = line.find_first_of(' ', 0);
		// get key
		key = line.substr(0, posSpace);
		// get value
		value = line.substr(posSpace + 1);

		// ckeck for valid key value pairs
		if (key.compare(CLAMAVIMC_CONF_CLAMCONF_PATH) == 0) {
			this->clamconfPath = value;
		} else if (key.compare(CLAMAVIMC_CONF_CLAMD_SOCKET) == 0) {
			this->clamdSocketPath = value;
		} else {
			LOG4CXX_DEBUG(logger, "Ignoring invalid configuration entry: " << line);
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ClamavIMC::checkOperationalStatus() {
	int s, t, len;
	struct sockaddr_un remote;
	int buflen = 100; // length of buffer
	char buffer[buflen]; // buffer for receiving the PONG
	std::string ping("PING"); // PING command as string
	std::string pong("PONG"); // PONG answer as string

	LOG4CXX_DEBUG(logger, "Checking Operational Status");

	// prepare struct sockaddr_un
	remote.sun_family = AF_UNIX;
	strcpy(remote.sun_path, this->clamdSocketPath.c_str());
	len = strlen(remote.sun_path) + sizeof(remote.sun_family);

	// socket
	if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		return;
	}

	// connect
	LOG4CXX_TRACE(logger, "Connecting to " << CLAMAVIMC_CONF_CLAMD_SOCKET);
	if (connect(s, (struct sockaddr *) &remote, len) == -1) {
		perror("connect");
		return;
	}

	// send
	LOG4CXX_TRACE(logger, "Sending PING");
	if (send(s, ping.c_str(), ping.length(), 0) == -1) {
		perror("send");
		return;
	}

	// recv read at most buflen bytes to buffer
	if ((t = recv(s, buffer, buflen, 0)) > 0) {
		// terminate c string, eliminate trailing LF
		buffer[t-1] = '\0';
		// check if we received the PONG
		if(pong.compare(buffer) == 0){
			LOG4CXX_TRACE(logger, "Received PONG. Clamd is running.");
			this->resultOperationalStatus = ClamavIMCLibrary::OPERATIONAL;
		} else {
			// no PONG received, but socket was available. assume that clamd
			// is installed but not operational
			LOG4CXX_TRACE(logger, "No Pong Received. Clamd is installed but not operational.");
			this->resultOperationalStatus = ClamavIMCLibrary::INSTALLED_NOT_OPERATIONAL;
		}
	} else {
		// receive error
		if (t < 0)
			perror("recv");
		else
			LOG4CXX_TRACE(logger, "Connection closed by server.");
	}

	// close socket
	close(s);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ClamavIMC::checkClamconf(){
	/* pid of forked process */
	pid_t pid;
	/* return value of child, observed by wait() */
	int rv;
	/* fd for the input & output of the pipe */
	int	commpipe[2];
	/* container for clamconf output*/
	std::stringstream ss;
	/* buffer for reading chunks of clamconf output */
	int buflen = 1000;
	char buffer[buflen];
	memset(buffer, 0, buflen);

	LOG4CXX_DEBUG(logger, "Executing clamconf");

	/* setup communication pipeline */
	if(pipe(commpipe)){
		perror("pipe");
		return;
	}

	/* attempt to fork and check for errors */
	if( (pid=fork()) == -1){
		perror("fork");
		return;
	}

	if(pid){
		/* a positive (non-negative) PID indicates the parent process */
		int tmp;
		/* close unused side of pipe (in side) */
		close(commpipe[1]);
		/* Wait for child process to end */
		wait(&rv);
		// check exit code of child
		if(!rv){
			LOG4CXX_TRACE(logger, "Clamconf exited successfully.")
			/* read output of child and write it to stream */
			while((tmp = read(commpipe[0], buffer, sizeof(buffer))) > 0){
				ss.write(buffer, tmp);
			}
			this->resultClamconf = ss.str();
		} else {
			// something went wrong. ignore clamconf output.
			LOG4CXX_DEBUG(logger, "Clamconf exited unsuccessfully. Output of clamconf will be ignored.")
		}
	}
	else{
		/* A zero PID indicates that this is the child process */
		/* Replace stdout with out side of the pipe */
		dup2(commpipe[1],1);
		/* Set non-buffered output on stdout */
		/* Close unused side of pipe (in side) */
		close(commpipe[0]);
		/* Replace the child fork with a new process */
		if(execl(this->clamconfPath.c_str(), this->clamconfPath.c_str(), (char *)NULL) == -1){
			perror("execl");
			exit(1);
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ClamavIMC::parseResults(){
	unsigned int pos1, pos2;
	std::string tmp;

	LOG4CXX_DEBUG(logger, "parsing results");

	// parse resultOperationalStatus
	this->out << "OperationalStatus ";
	switch(this->resultOperationalStatus){
		case ClamavIMCLibrary::UNKNOWN:
			this->out << "unknown" << std::endl;
			break;
		case ClamavIMCLibrary::NOT_INSTALLED:
			this->out << "not_installed" << std::endl;
			break;
		case ClamavIMCLibrary::INSTALLED_NOT_OPERATIONAL:
			this->out << "installed_not_operational" << std::endl;
			break;
		case ClamavIMCLibrary::OPERATIONAL:
			this->out << "operational" << std::endl;
			break;
		case ClamavIMCLibrary::RESERVED:
			this->out << "reserved" << std::endl;
			break;
		default:
			this->out << "unknown" << std::endl;
			break;
	}

	// parse resultClamconf
	if(this->resultClamconf.size() > 0) {
		// find Version
		pos1 = this->resultClamconf.find("Version:", 0); // beginning of line
		pos2 = this->resultClamconf.find_first_of("\n", pos1); // end of line
		tmp = this->resultClamconf.substr(pos1, pos2-pos1); // complete line
		this->out << tmp << std::endl;
		LOG4CXX_DEBUG(logger, tmp);

		// find main.cvd entry
		pos1 = this->resultClamconf.find("main.cvd:", 0); // beginning of line
		if( pos1 == std::string::npos ) {
			//For incremental updates filname ends with .cld
			pos1 = this->resultClamconf.find("main.cld:", 0); // beginning of line
		}
		pos2 = this->resultClamconf.find_first_of("\n", pos1); // end of line
		tmp = this->resultClamconf.substr(pos1, pos2-pos1); // complete line
		this->out << tmp << std::endl;
		LOG4CXX_DEBUG(logger, tmp);

		// find daily.cvd entry
		pos1 = this->resultClamconf.find("daily.cvd:", 0); // beginning of line
		if( pos1 == std::string::npos ) {
			//For incremental updates filname ends with .cld
			pos1 = this->resultClamconf.find("daily.cld:", 0); // beginning of line
		}
		pos2 = this->resultClamconf.find_first_of("\n", pos1); // end of line
		tmp = this->resultClamconf.substr(pos1, pos2-pos1); // complete line
		this->out << tmp << std::endl;
		LOG4CXX_DEBUG(logger, tmp);
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ClamavIMC::measure(){
	LOG4CXX_DEBUG(logger, "Measuring ClamAV");
	this->out.clear();
	this->out.str("");
	this->readConfiguration();
	this->checkOperationalStatus();
	this->checkClamconf();
	this->parseResults();
//	LOG4CXX_TRACE(logger, out.str());
}
