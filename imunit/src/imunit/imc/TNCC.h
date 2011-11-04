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
 
#ifndef TNCFHH_IML_TNCC_H_
#define TNCFHH_IML_TNCC_H_

#include "../exception/ResultException.h"

#include <tcg/tnc/tncifimc.h>

namespace tncfhh {

namespace iml {

// Prototyp instead of include IMCLibrary.h
class IMCLibrary;

/**
 * Interfaceclass for the TNC-Client. With this class, you can call
 * functions on TNC-Client.
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>09.02.2008 - create class (mbs)</li>
 *   <li>08.05.2008 - change namespace (mbs)</li>
 *   <li>16.06.2009 - redesign (ib)</li>
 * </ul>
 *
 * @class TNCC
 * @brief Interfaceclass for the TNC-Client.
 * @date 09.02.2008
 * @author Mike Steinmetz (mbs)
 *         Ingo Bente (ib)
 */
class TNCC
{
private:
	/**
	 * The connection ID
	 */
	TNC_ConnectionID cid;

	/**
	 * The IMCLibrary for the corresponding IMC instance of this TNCC instance.
	 */
	IMCLibrary *pImcLibrary;

public:
	/**
	 * Ctor.
	 *
	 * @param cid the connection ID
	 * @param cid the corresponding IMCLibrary
	 */
	TNCC(TNC_ConnectionID cid, IMCLibrary *pImcLibrary);

	/**
	 * Dtor.
	 */
	virtual ~TNCC();

	// can not be called by an IMC outside of provideBindFunction.
//	virtual void reportMessageTypes() throw (exception::Exception);

	/**
	 * send a IMV/IMC message
	 *
	 * @param message The binary message you want to send.
	 * @param messageLength The length of message.
	 * @param messageType The type of message.
	 *
	 * @see <a href="https://www.trustedcomputinggroup.org/specs/TNC/TNC_IFIMC_v1_2_r8.pdf">TCG TNC IF-IMC Specification 1.2</a>
	 */
	virtual void sendMessage(TNC_BufferReference message, TNC_UInt32 messageLength, TNC_MessageType messageType) throw (ResultException);

	/**
	 * send a IMV/IMC message
	 *
	 * @param message The binary message you want to send.
	 * @param messageLength The length of message.
	 * @param vendorId The type of message.
	 * @param messageSubtype The type of message.
	 *
	 * @see <a href="https://www.trustedcomputinggroup.org/specs/TNC/TNC_IFIMC_v1_2_r8.pdf">TCG TNC IF-IMC Specification 1.2</a>
	 */
	virtual void sendMessage(TNC_BufferReference message, TNC_UInt32 messageLength, TNC_VendorID vendorId, TNC_MessageSubtype messageSubtype) throw (ResultException);

	/**
	 * Call the TNCC and request to retry the TNC handshake.
	 *
	 * @param Reason the reason of handshake retry
	 *
	 * @see <a href="https://www.trustedcomputinggroup.org/specs/TNC/TNC_IFIMC_v1_2_r8.pdf">TCG TNC IF-IMC Specification 1.2</a>
	 */
	virtual void requestHandshakeRetry(TNC_RetryReason reason) throw (ResultException);
};

}  // namespace iml

}  // namespace tncfhh

#endif /*TNCFHH_IML_TNCC_H_*/
