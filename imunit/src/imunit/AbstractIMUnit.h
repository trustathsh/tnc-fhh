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
 
#include "IMUnitLibrary.h"

#ifndef TNCUTIL_ABSTRACTIMUNIT_H_
#define TNCUTIL_ABSTRACTIMUNIT_H_

namespace tncfhh {

namespace iml {

/**
 * AbstractIMUnit represents the similarities of IMCs and IMVs. Known
 * subclasses are AbstractIMC and AbstractIMV. There is one instance
 * of this class per connection.
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>07.02.2008 - create class (ib)</li>
 *   <li>08.05.2008 - change namespace (mbs)</li>
 *   <li>27.03.2009 - add round (mbs)</li>
 *   <li>28.07.2009 - redesign 0.6.0 (ib)</li>
 * </ul>
 *
 * @class AbstractIMUnit
 * @brief Represents the abstract super class for any concrete IMC/IMV instance.
 * @date 07.02.2008
 * @author Ingo Bente
 * @author Mike Steinmetz (mbs)
 */
class AbstractIMUnit {

	/*
	 * Friends of AbstractIMUnit (are allowed to call private methods)
	 * FIXME AbstractIMUnit notifyConnectionChange() + friends
	 */
	// for accessing notifyConnectionChange(TNC_ConnectionState newState)
	friend class AbstractIMV;
	friend class AbstractIMC;
	// for accessing this->round
	friend class IMCLibrary;
	friend class IMVLibrary;

public:

	/**
	 * Ctor.
	 *
	 * @param conID The TNC_ConnectionID for the IMC or IMV instance.
	 */
	AbstractIMUnit(TNC_ConnectionID conID);

	/**
	 * Dtor.
	 */
	virtual ~AbstractIMUnit();

	/**
	 * Getter for @link {connectionState}
	 *
	 * @return @link {connectionState}
	 */
	TNC_ConnectionState getConnectionState() const;

	/**
	 * Getter for @link {connectionID}
	 *
	 * @return @link {connectionID}
	 */
	TNC_ConnectionID getConnectionID() const;

	/**
	 * Getter for @link {round}
	 *
	 * @return @link {round}
	 */
	TNC_UInt32 getRound() const;

	/**
	 * Hook method for event notification. Is called when the status of the
	 * connection changes. Should be overwritten by the concrete IMC/IMV
	 * implementation. The new state is already saved in this->connectionState
	 * when the method is called.
	 *
	 * @return A result indicating whether the operation was successful or not.
	 */
	virtual TNC_Result notifyConnectionChange();

	/**
	 * Invoked after TNC_IMC_BatchEnding(...) has been called by TNC client/server.
	 * Has to be implemented by a concrete IMC/IMV. All messages received in a batch
	 * have been delivered to the IMC/IMV. Last chance for IMC/IMV to send a message
     * to IMV/IMC.
     *
	 * @return A result indicating whether the operation was successful or not.
	 */
	virtual TNC_Result batchEnding();

	/**
	 * Invoked after TNC_IMV_ReceiveMessage() has been called by TNC server.
	 * Has to be implemented by a concrete IMV.
	 * This function is called to deliver a message to the respective IMV.
	 *
	 * @param message The message that shall be delivered to the IMV.
	 * @param messageType The type of the message.
	 * @param messageLength The number of octets in the message.
	 *
	 * @return A result indicating whether the operation was succseful or not.
	 */
	virtual TNC_Result receiveMessage(TNC_BufferReference message,
			TNC_UInt32 messageLength, TNC_MessageType messageType);

private:
	/**
	 * Invoked after TNC_IMC/V_NotifyConnectionChange(). Handles errors and
	 * saves the new connection state to this. Calls @link {notifyConnectionChange()}
	 *
	 * @param newState The new connection state.
	 */
	virtual TNC_Result notifyConnectionChange(TNC_ConnectionState newState);

	/**
	 * Represents the current connection state. Possible values are defined in
	 * tncifimc/v.h.
	 */
	TNC_ConnectionState connectionState;

	/**
	 * Connection ID for this IMC/IMV.
	 */
	const TNC_ConnectionID connectionID;

	/**
	 * Counter for the rounds in a TNC handshake. This value is changed by the
	 * corresponding IMC/VLibrary object as follows:
	 * - set to 0 at the end of IMC/VLibrary::notifyConnectionChange() when
	 *   called with newState == TNC_CONNECTION_STATE_HANDSHAKE
	 * - for IMC/V  : increased before IMC/VLibrary::batchEnding returns
	 * - for the IMC: increased before IMCLibrary::beginHandshake returns
	 */
	TNC_UInt32 round;
};

}  // namespace iml

} // namespace tncfhh

#endif /*TNCUTIL_ABSTRACTIMUNIT_H_*/
