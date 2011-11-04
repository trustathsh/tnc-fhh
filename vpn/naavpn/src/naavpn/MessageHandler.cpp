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
 
#include "MessageHandler.h"
#include "PEPNotifier.h"

#include <log4cxx/logger.h>

#define LOGNAME "[MsgHandler]"
static log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger(LOGNAME);

namespace tnc {

using namespace ifttls;

MessageHandler::MessageHandler(tnc::Endpoint *ep)
	: ep(ep), reply_msg(0), pre_negotiation_finished(false),
	  want_read_(true), want_write_(false), finished_(false) {


	std::string ip = ep->getAddress()->to_string();

	// Create connection id
	conn_id = tncfhh::iel::coordinator.getConnection(ip);
}

MessageHandler::~MessageHandler() {
	LOG4CXX_TRACE(logger, "Destructor");
	clean();
}

void
MessageHandler::handleMessage(Envelope* message) {

	// Ensure reply_msg is clean!
	clean();

	switch (message->getMessageType()) {
		case Envelope::IFT_TNCCS_20_BATCH: handleTnccs20Batch(message); break;
	}
}

void
MessageHandler::handleTnccs20Batch(Envelope* message) {
	LOG4CXX_TRACE(logger, "TNCCS 20 Batch received");

	// The notifier is used to send a recommendation
	// to the PEP
	tnc::PEPNotifier notifier;

	try {
		tncfhh::iel::TNCCSData data((TNC_BufferReference) message->getMessageValue(),
				                    message->getMessageValueLength());

		// Passing the TNCCS payload to the TNC server
		tncfhh::iel::TNCCSData ret = tncfhh::iel::coordinator.processTNCCSData(conn_id, data);


		// This part is only reached when the TNC server wants to send data
		// back to the client since processTNCCSData() throws an exception
		// if the Endpoint verification has finished.
		reply_msg = new Envelope(Envelope::IFT_TNCCS_20_BATCH,
				       (const char*) ret.getData(),
					ret.getLength());

	} catch (tncfhh::iel::ConnectionStateExceptionWithTNCCSData e) {
		LOG4CXX_INFO(logger, "Access Recommendation");

		switch (e.getConnectionState()) {
		case TNC_CONNECTION_STATE_ACCESS_ALLOWED:
			LOG4CXX_INFO(logger, "ALLOW");
			notifier.sendAllow(ep);
			break;
		case TNC_CONNECTION_STATE_ACCESS_NONE:
			LOG4CXX_INFO(logger, "NO ACCESS");
			notifier.sendDeny(ep);
			break;
		case TNC_CONNECTION_STATE_ACCESS_ISOLATED:
			LOG4CXX_INFO(logger, "ISOLATE");
			// Not implemented yet!
			break;
		}
		tncfhh::iel::TNCCSData ret = e.getLastTnccsData();
		reply_msg = new Envelope(Envelope::IFT_TNCCS_20_BATCH,
				       (const char*) ret.getData(),
					ret.getLength());
		finished_ = true;
	}
}


bool
MessageHandler::hasReplyMessage() const {
	return reply_msg != 0;
}


Envelope*
MessageHandler::getReplyMessage() {
	Envelope *tmp = reply_msg;
	reply_msg = NULL;
	return tmp;
}

void
MessageHandler::clean() {

	if (reply_msg != NULL) {
		delete reply_msg;
		reply_msg = 0;
	}

}


bool
MessageHandler::wantWrite() const {
	return want_write_;
}


bool
MessageHandler::wantRead() const {
	return want_read_;
}


void
MessageHandler::done() {
	if (!finished_) {
		want_read_ = !want_read_;
		want_write_ = !want_write_;
	} else {
		want_read_ = false;
		want_write_ = false;
	}
}

}
