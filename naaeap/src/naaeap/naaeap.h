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
 
/**
 * TNC@FHH specific addons to the TCG specifications
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>15.08.2006 - create (MS)</li>
 *   <li>15.02.2008 - comments/change (ib)</li>
 *   <li>11.04.2008 - add connectionCreate (mbs)</li>
 *   <li>04.11.2008 - removed obsolete function pointers (bhl)</li>
 *   <li>04.11.2008 - changed the name from 'fhhaddon.h' to 'naatncs.h' (bhl)</li>
 *   <li>08.07.2009 - redesign 0.6.0 (ib)</li>
 *   <li>22.07.2009 - move some define to tcg/tnc/fhhaddon.h (mbs)</li>
 * </ul>
 *
 * @date 15.08.2006
 * @author Martin Schmiedel (MS)
 * @author: Mike Steinmetz (mbs)
 * @author: Ingo Bente (ib)
 * @author: Bastian Hellmann (bhl)
 */
#ifndef NAAEAP_H_
#define NAAEAP_H_

#include <tcg/tnc/tncifimv.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
#ifdef TNC_IMV_EXPORTS
#define TNC_IMV_API __declspec(dllexport)
#else
#define TNC_IMV_API __declspec(dllimport)
#endif
#else
#define TNC_IMV_API
#endif

/**
 * Terminate function for libNAAEAP.
 */
TNC_IMV_API TNC_Result terminate();

/**
 * Interface function for libNAAEAP.
 *
 * Called by FreeRADIUS when a new connection // FIXME - documentation
 * should be established (necessary for running a TNC handshake).
 * This function sets up a new connection context that is identified
 * by the given connection id.
 *
 * @param tnc_config path to tnc_config file
 */
TNC_IMV_API TNC_Result initialize(/* in */ const char* tnc_config);

/**
 * Interface function for libNAAEAP.
 *
 * Called by FreeRADIUS when a new connection // FIXME - documentation
 * should be established (necessary for running a TNC handshake).
 * This function sets up a new connection context that is identified
 * by the given connection id.<br/>
 * Use the default tnc_config file.
 */
TNC_IMV_API TNC_Result initializeDefault();

/**
 * Interface function for libNAAEAP.
 *
 * Called by FreeRADIUS when a new connection
 * should be established (necessary for running a TNC handshake).
 * This function sets up a new connection context that is identified
 * by the given connection id.
 *
 * @param input c string as identifying key for the connection
 * @param connedtionID buffer where the new connection ID is saved
 */
TNC_IMV_API TNC_Result getConnection(/* in */ const char* input,
		 /* out */ TNC_ConnectionID *connectionID);

/**
 * Interface function for libNAAEAP.
 *
 * Called by FreeRADIUS when EAP-TNC data arrives.
 *
 * @param conID The connection ID
 * @param input The complete EAP_TNC packet (all EAP_TNC specific data) received from the peer
 * @param inputLength The length of the incoming EAP_TNC packet
 * @param output The EAP_TNC packet this library has generated as response to the input packet
 * @param outputLength The length of the ouput packet
 * @param connectionState The current TNC connection state. Determines which EAP code is set in the packet
 *        that will be send to the peer.
 */
TNC_IMV_API TNC_Result processEAPTNCData(
        /*in*/ TNC_ConnectionID conId,
        /*in*/ TNC_BufferReference input,
        /*in*/ TNC_UInt32 inputLength,
        /*out*/ TNC_BufferReference *output,
        /*out*/ TNC_UInt32 *outputLength,
        /*out*/ TNC_ConnectionState *connectionState
);

/**
 * Interface function to libNAAEAP.
 *
 * Called by FreeRADIUS to store the username with a connection.
 *
 * @param conID The connection ID
 * @param username The username
 */
TNC_IMV_API TNC_Result storeUsername(
		/*in*/ TNC_ConnectionID conID,
		/*in*/ TNC_BufferReference username,
		/*in*/ TNC_UInt32 usernameLength
);

/**
 * Activate or deactivate the fragmentation.
 *
 * @param active FALSE (0) deactivate fragmentation, otherwise activate fragmentation.
 */
void setActivateFragmentation(int active);

/**
 * Activate or deactivate the fragmentation.
 *
 * @return FALSE (0) if fragmentation is deactivate and TRUE (1) if fragmentation is activate.
 */
int getActivateFragmentation();

#ifdef __cplusplus
}
#endif

#endif /*NAAEAP_H_*/

