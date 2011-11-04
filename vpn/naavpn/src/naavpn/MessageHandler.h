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
 
#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <tncs/iel/Coordinator.h>

#include <tncvpn/ifttls/IFTMessages.h>
#include <tncvpn/iel/Endpoint.h>

namespace tnc {

/**
 * The message_handler is responsible for processing any
 * incoming IF-T message, independent of the type(negotiation
 * or TNCCS data).
 *
 * Only this class needs to get enhanced in order to support the
 * negotiation of further parameters.
 */
class MessageHandler {
private:

	/** The connection id for the TNC server */
	TNC_ConnectionID conn_id;
	tnc::Endpoint *ep;

	ifttls::Envelope* reply_msg;

	/** Indicates that the pre-negotiation phase has finished */
	bool pre_negotiation_finished;

	/** Indicates that the TNC server wants to read */
	bool want_read_;

	/** Indicates that the TNC server wants to write */
	bool want_write_;

	bool finished_;

	void clean();


protected:

	/**
	 * Handles and processes a TNCCS 2.0 message.
	 */
	void handleTnccs20Batch(ifttls::Envelope* message);


public:
	/**
	 * Create a message_handler.
	 */
	MessageHandler(tnc::Endpoint *ep);
	virtual ~MessageHandler();

	/**
	 * Handles any kind of IF-T binding to TLS
	 * message and invokes the corresponding method
	 * to process the message.
	 */
	void handleMessage(ifttls::Envelope* message);

	/**
	 * Indicates whether there is reply message.
	 */
	bool hasReplyMessage() const;

	/**
	 * Returns the IF-T message which shall be sent
	 * to the Access Requestor.
	 */
	ifttls::Envelope* getReplyMessage();


	/**
	 * Indicates that the TNC server wants to write.
	 */
	bool wantWrite() const;

	/**
	 * Indicates that the TNC server wants to read.
	 */
	bool wantRead() const;

	bool isFinished() const { return finished_; }


	/**
	 * This method tells the message_handler that the data
	 * was successfully sent by the underlying network layer.
	 */
	void done();
};

}
#endif
