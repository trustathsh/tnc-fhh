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
 * IFTTLSConn.cpp
 *
 *  Created on: Aug 1, 2010
 *      Author: awelzel
 */
#include "tncvpn/ifttls/IFTTLSConn.h"
#include "tncvpn/ifttls/exceptions/ReceiveException.h"
#include "tncvpn/ifttls/exceptions/VersionException.h"

/* general stuff */
#include <arpa/inet.h>

/* logging stuff */
#include <log4cxx/logger.h>
#define LOGNAME "[IFTTLSConn]"
static log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger(LOGNAME);

static void output_hex(const char *msg, size_t size);

using namespace boost::asio;
namespace ifttls {


IFTTLSConnection::IFTTLSConnection(ssl::stream<ip::tcp::socket>& sock) :
		socket_(sock),
		curMessage(NULL),
		curMsgLength(0),
		curIdentifier(0)
{
	LOG4CXX_TRACE(logger, "Constructor");
	memset(curHeader, 0, Envelope::IFT_HEADER_LENGTH);
}
IFTTLSConnection::~IFTTLSConnection()
{
	LOG4CXX_TRACE(logger, "Destructor");
}

/**
 * TODO: as soon as more versions are out there, this need revisiting
 */
void IFTTLSConnection::startVersionNegotiation()
{
	LOG4CXX_DEBUG(logger, "Client side Version Negotiation...");
	// version 1
	VersionRequestMessage vrm;
	uint32_t payloadLen = vrm.getLength();
	char *payload = vrm.getMessage();
	Envelope *env = new Envelope(Envelope::IFT_VERSION_REQUEST, payload, payloadLen);
	sendMessage(env);

	/* free memory of message */
	delete[] payload;
	delete env;
	payload = NULL; env = NULL;

	/* get the version response */
	env = receiveMessage();
	payload = env->getMessageValue();
	VersionResponseMessage vrsp(payload);
	delete[] payload;
	delete env;
	payload = NULL; env = NULL;

	if (vrsp.getVersion() != 1) {
		LOG4CXX_ERROR(logger, "Unsupported Version received");
		throw VersionException("Unsupported Version received");
	}
	LOG4CXX_DEBUG(logger, "Client side Version Negotiation Finished... ");
}



/**
 * TODO: as soon as more versions are out there, this needs revisiting
 */
void IFTTLSConnection::expectVersionNegotiation()
{
	LOG4CXX_DEBUG(logger, "Server side Version Negotiation...");
	Envelope *env = receiveMessage();
	char *payload = env->getMessageValue();
	VersionRequestMessage vrm(payload);
	delete[] payload;
	delete env;
	payload = NULL; env = NULL;

	if (vrm.getMinVersion() > 1 || 1 < vrm.getMaxVersion()) {
		LOG4CXX_ERROR(logger, "Unsupported Version in request");
		// TODO: send back an error message
		throw VersionException("Unsupported Version in request");
	} else {
		VersionResponseMessage vresp(1);
		payload = vresp.getMessage();
		Envelope resp(Envelope::IFT_VERSION_RESPONSE, payload, vrm.getLength());
		delete[] payload;
		payload = NULL;
		sendMessage(&resp);
	}
	LOG4CXX_DEBUG(logger, "Server side Version Negotiation Finished... ");
}


void IFTTLSConnection::sendMessage(Envelope *env)
{
	env->setIdentifier(getNextIdentifier());
	LOG4CXX_DEBUG(logger, "Sending a message... Length=" << env->getMessageLength()
			<< " Identifier=" << env->getIdentifier());
	char *msg = env->getMessage();
	uint32_t length = env->getMessageLength();
	output_hex((const char*)msg, length);
	boost::asio::write(socket_, boost::asio::buffer(msg, length));
	delete[] msg;
}
Envelope *IFTTLSConnection::receiveMessage()
{
	LOG4CXX_DEBUG(logger, "Receiving a message...");
	Envelope *env = NULL;
	try {
		readHeader();
		allocateMessageBuffer();
		readRestOfMessage();
		env = constructEnvelope();
		output_hex((const char *)curMessage, curMsgLength);
		freeBuffers();
	} catch (ReceiveException e) {
		LOG4CXX_ERROR(logger, e.getMessage());
		freeBuffers();
		throw e;
	} catch (...) {
		// TODO: could be more specific
		LOG4CXX_ERROR(logger, "Oh well, something bad happend...");
		freeBuffers();
		throw ReceiveException("Unknown Exception catched");
	}
	return env;
}

void IFTTLSConnection::readHeader()
{
	//boost::asio::buffer buf(curHeader, Envelope::IFT_HEADER_LENGTH);
	size_t read = boost::asio::read(socket_,
			boost::asio::buffer(curHeader, Envelope::IFT_HEADER_LENGTH));

	if (read != Envelope::IFT_HEADER_LENGTH) {
		LOG4CXX_ERROR(logger, "Failed to read header!");
		throw ReceiveException(std::string("Failed to read header."));
	}
	curMsgLength = ntohl(*((uint32_t *)(curHeader + Envelope::IFT_HEADER_LENGTH_OFFSET)));
}


void IFTTLSConnection::allocateMessageBuffer()
{
	if (curMsgLength < 16 || curMessage != NULL) {
		throw ReceiveException("Length < 16 or curMessage not deleted.");
	}

	/* allocate memory for the whole message */
	curMessage = new uint8_t[curMsgLength];

	/* copy the header in the message buffer */
	memcpy(curMessage, curHeader, Envelope::IFT_HEADER_LENGTH);

	/* zero out the header buffer */
	memset(curHeader, 0, Envelope::IFT_HEADER_LENGTH);

}
void IFTTLSConnection::readRestOfMessage()
{
	uint32_t rest = curMsgLength - Envelope::IFT_HEADER_LENGTH;

	if (rest > 0) {
		size_t read = boost::asio::read(socket_,
			boost::asio::buffer(curMessage + Envelope::IFT_HEADER_LENGTH, rest));
	}
}

Envelope *IFTTLSConnection::constructEnvelope()
{
	Envelope *env = new Envelope((const char*)curMessage, curMsgLength);
	return env;
}

void IFTTLSConnection::freeBuffers()
{
	curMsgLength = 0;
	delete[] curMessage;
	curMessage = NULL;
}

}

/*
 * how i hate that stuff :-(
 */
static void output_hex(const char *msg, size_t size)
{
	char num[3];
	std::string output;

	if (size == 0)
		return;

	int curIndex = 0;
	int row = 0;
	int rows = (size / 16) + 1;
	int curCount = 0;
	int smallCount = 0;

	for (int i = 0; i < size; i++) {
		snprintf(num, 3, "%02hhx", msg[i]);
		output.append(num);
		curCount++;
		smallCount++;

		if (curCount == 16) {
			output.append(2, ' ');
			for (int j = 16; j > 0; j--) {
				if (isprint(msg[i - j]) && !isspace(msg[i - j])) {
					output.append(1, msg[i - j]);
				} else {
					output.append(".");
				}
			}
			LOG4CXX_TRACE(logger, output);
			smallCount = 0;
			curCount = 0;
			output.clear();
		} else if (smallCount == 2) {
			output.append(1, ' ');
			smallCount = 0;
		}
	}

	// nobody should ask...
	output.append(2 * (16 - curCount) + (16 - curCount) / 2 + 2, ' ');

	for (int j = curCount; j > 0; j--) {
		if (isprint(msg[size - j]) && !isspace(msg[size - j])) {
			output.append(1, msg[size - j]);
		} else {
			output.append(".");
		}
	}
	LOG4CXX_TRACE(logger, output);
}

