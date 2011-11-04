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
 
#include "naaeap.h"
#include "logic/Defragmentation.h"
#include "logic/Fragmentation.h"
#include <tncs/iel/Coordinator.h>
#include <tncs/iel/TNCCSData.h>
#include <tncs/exception/ConnectionStateException.h>
#include <tncfhhConfig.h>

#include <iostream>
#include <sstream>
#include <string>
#include <string.h>
#include <stdlib.h>

// include log4cxx header files.
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/helpers/exception.h>

using namespace log4cxx;

/*
 * Interface for FreeRADIUS etc.
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>15.02.2008 - create file new (mbs)</li>
 *   <li>11.04.2008 - add connectionCreate (mbs)</li>
 *   <li>09.07.2009 - redesign 0.6.0 (ib)</li>
 * </ul>
 *
 * @date 15.02.2008
 * @author Mike Steinmetz (mbs)
 *         Ingo Bente (ib)
 */

namespace tncfhh {

namespace naaeap {

static LoggerPtr logger(Logger::getLogger("NAA-EAP.naaeap"));

/**
 * Singleton defragmentation
 */
static tncfhh::nal::Defragmentation deframentation;

/**
 * Singleton fragmentation
 */
static tncfhh::nal::Fragmentation fragmentation;

}  // namespace naaeap

} // namespace tncfhh

using namespace tncfhh::naaeap;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_IMV_API TNC_Result terminate()
{
	tncfhh::iel::coordinator.terminate();
    return TNC_RESULT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_IMV_API TNC_Result initialize(/* in */ const char* tnc_config)
{
	LOG4CXX_INFO(logger, "initialize naaeap version " << TNCFHH_VERSION_MAJOR << "." << TNCFHH_VERSION_MINOR << "." << TNCFHH_VERSION_PATCH << " with tnc_config file \"" << tnc_config << "\"");

    // check parameters
    if(tnc_config == NULL)
    {
    	LOG4CXX_WARN(logger, "Invalid parameters: tnc_config is " << tnc_config);
    	return TNC_RESULT_INVALID_PARAMETER;
    }

    try {
		tncfhh::iel::coordinator.initialize(std::string(tnc_config));
	} catch (std::domain_error &e) {
		LOG4CXX_WARN(logger, e.what());
		// no error all works smoothly!
	}
    return TNC_RESULT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_IMV_API TNC_Result initializeDefault()
{
	LOG4CXX_INFO(logger, "initialize naaeap version " << TNCFHH_VERSION_MAJOR << "." << TNCFHH_VERSION_MINOR << "." << TNCFHH_VERSION_PATCH << " with default tnc_config file.");

    try {
		tncfhh::iel::coordinator.initialize();
	} catch (std::domain_error &e) {
		LOG4CXX_WARN(logger, e.what());
		// no error all works smoothly!
	}
    return TNC_RESULT_SUCCESS;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result getConnection(/* in */ const char* input,
						 /* out */ TNC_ConnectionID *connectionID)
{
    LOG4CXX_INFO(logger, "getConnection input is " << input);

    // check parameters
    if(input == NULL || connectionID == NULL)
    {
    	LOG4CXX_WARN(logger, "Invalid parameters: input is " << input << " connectionID is " << connectionID);
    	return TNC_RESULT_INVALID_PARAMETER;
    }

	try {
		// call TNCS for new connection
		TNC_ConnectionID conID = tncfhh::iel::coordinator.getConnection(std::string(input));

        // reset old connection if one exists
		deframentation.reset(conID);
		fragmentation.reset(conID);

		// set connectionID (used by freeradius)
		*connectionID = conID;

	} catch (std::exception &e) {
		LOG4CXX_ERROR(logger, e.what());
		return TNC_RESULT_FATAL;
	} catch (...) {
		LOG4CXX_ERROR(logger, "UNKOWN ERROR!");
		return TNC_RESULT_FATAL;
	}

	return TNC_RESULT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void setOutput(
		/*in*/ tncfhh::nal::EapTncFragment & data,
		/*out*/ TNC_BufferReference *output,
        /*out*/ TNC_UInt32 *outputLength)
{
	// TNCUtil::io::printBinaryOut(tnccsBatchFragmentSend->getBatchFragment(), tnccsBatchFragmentSend->getBatchFragmentLength(), "want to send tnccsBatchFragment to FreeRADIUS "); // FIXME - binary logging
	*output = (TNC_BufferReference)malloc(sizeof(TNC_Buffer)*data.getFragmentLength());
	memcpy(*output, data.getFragment(), data.getFragmentLength());
	*outputLength = data.getFragmentLength();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result processEAPTNCData(
		/*in*/ TNC_ConnectionID conID,
        /*in*/ TNC_BufferReference input,
        /*in*/ TNC_UInt32 inputLength,
        /*out*/ TNC_BufferReference *output,
        /*out*/ TNC_UInt32 *outputLength,
        /*out*/ TNC_ConnectionState *connectionState)
{
	LOG4CXX_INFO(logger, "processEAPTNCData for connection " << conID);

    /* if debugging then print receive EAP-TNC-Package */ // FIXME - binary logging
//    if (log.isDebug()) {
//    	std::stringstream s;
//    	TNCUtil::io::printBinaryOut(input, inputLength, "Receive EAP-TNC package from FreeRADIUS: ", s);
//    	log.debug(s.str());
//    }

    /* check on inputLength error*/
    if (inputLength < 1)
    {
    	LOG4CXX_ERROR(logger, "Invalid parameter: inputLength < 1");
    	return TNC_RESULT_INVALID_PARAMETER;
    }

    /* read TNC flags */
    bool lengthInclude = (*input)&0x80             /*10000000*/;
	//bool moreFragments = (*input)&0x40           /*01000000*/;
	bool start = (*input)&0x20                     /*00100000*/;
	//bool diffieHellmanPreNegotion = (*input)&0x10/*00010000*/;
	//RESERVED                                     /*00001000*/;
	unsigned char version = (*input)&0x07          /*00000111*/;

	/* check on start bit error */
    if (start)
    {
        LOG4CXX_ERROR(logger, "Invalid parameter: Start bit is set.");
        return TNC_RESULT_INVALID_PARAMETER;
    }

    /* check on version */
    if (version != 1)
    {
    	LOG4CXX_ERROR(logger, "Invalid version: " << version);
        return TNC_RESULT_INVALID_PARAMETER;
    }

    /* check on length include bit is set an input length is lesser than five error */
    if (lengthInclude && inputLength < 5)
    {
    	LOG4CXX_ERROR(logger, "Invalid parameter: Length bit is included, but EAP-Data length is " << inputLength);
        return TNC_RESULT_INVALID_PARAMETER;
    }

    /* calculate TNC data length */
    TNC_UInt32 tncDataLength = 0;
    if (lengthInclude)
    	tncDataLength = inputLength - EAP_TNC_HEADER_FLAGS_SIZE - EAP_TNC_HEADER_DATA_LENGTH_SIZE;
    else
    	tncDataLength = inputLength - EAP_TNC_HEADER_FLAGS_SIZE;

    /* incoming fragment */
	tncfhh::nal::EapTncFragment incommingFragment(input, inputLength);
	tncfhh::iel::TNCCSData outgoingTnccsData(0);

    try {
	    if (tncDataLength == 0 && tncfhh::iel::coordinator.getConnectionState(conID)==TNC_CONNECTION_STATE_HANDSHAKE) {
    		LOG4CXX_INFO(logger, "Received ACK. Sending next Fragment.");

    		tncfhh::nal::EapTncFragment outgoingFragment = tncfhh::naaeap::fragmentation.getNextFragment(conID);
    		setOutput(outgoingFragment, output, outputLength);
    		*connectionState = TNC_CONNECTION_STATE_HANDSHAKE;
	    } else if (tncDataLength == 0) {
	    	LOG4CXX_INFO(logger, "Received Last 'ACK' EAP-TNC data packet.");
	    	throw tncfhh::iel::ConnectionStateException(tncfhh::iel::coordinator.getConnectionState(conID));
	    } else {
	    	LOG4CXX_INFO(logger, "Received EAP-TNC data packet.");

			tncfhh::iel::TNCCSData incommingTnccsData = deframentation.defragment(conID, incommingFragment);
			LOG4CXX_INFO(logger, "Send complete TNCCSData packet to IEL.");

			outgoingTnccsData = tncfhh::iel::coordinator.processTNCCSData(conID, incommingTnccsData);
			tncfhh::nal::EapTncFragment outgoingFragment = fragmentation.fragment(conID, outgoingTnccsData);
			setOutput(outgoingFragment, output, outputLength);
			*connectionState = TNC_CONNECTION_STATE_HANDSHAKE;
	    }
	} catch (tncfhh::nal::AcknowledgementException & e) {
		LOG4CXX_INFO(logger, "Received fragment and fragmentation is going on. Sending ACK.");
		*output = (TNC_BufferReference)malloc(sizeof(TNC_BufferReference)); //allocate 1 byte for tnc-packet without data (ack)
		**output = 1; // the tnc-packet (only version is set)
		*outputLength = 1; //the tnc-packet-length (and eap-data-length)
		*connectionState = TNC_CONNECTION_STATE_HANDSHAKE;
	} catch (tncfhh::iel::ConnectionStateExceptionWithTNCCSData & cse) {
		LOG4CXX_INFO(logger, cse.what());
		outgoingTnccsData = cse.getLastTnccsData();
        tncfhh::nal::EapTncFragment outgoingFragment = fragmentation.fragment(conID, outgoingTnccsData);
		setOutput(outgoingFragment, output, outputLength);
		*connectionState = TNC_CONNECTION_STATE_HANDSHAKE;
	} catch (tncfhh::iel::ConnectionStateException & cse) {
		LOG4CXX_INFO(logger, cse.what());
		*connectionState = cse.getConnectionState();
	} catch (tncfhh::iel::ResultException & r) {
		LOG4CXX_INFO(logger, r.what());
		return r.getReturnValue();
	} catch (...) {
		LOG4CXX_ERROR(logger, "UNKNOWN ERROR!");
		return TNC_RESULT_FATAL;
	}

    return TNC_RESULT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result storeUsername(
		/*in*/ TNC_ConnectionID conID,
        /*in*/ TNC_BufferReference username,
        /*in*/ TNC_UInt32 usernameLength) {

	LOG4CXX_TRACE(logger, "storeUsername: username is " << username << ", connectionID is " << conID);

	// check parameters
	if(!username)
	{
		LOG4CXX_WARN(logger, "Invalid parameters: username is not set, connectionID is " << conID);
		return TNC_RESULT_INVALID_PARAMETER;
	}

	try {
		// IMV_ID is 0, as the username shall be stored for ALL IMVs respectively for the connection itself
		// FIXME IMV_ID == 0 is just a quick "hack" ...
		tncfhh::iel::coordinator.setAttribute(0, conID, TNC_ATTRIBUTEID_USERNAME, usernameLength, username);
	} catch (std::exception &e) {
		LOG4CXX_ERROR(logger, e.what());
		return TNC_RESULT_FATAL;
	} catch (...) {
		LOG4CXX_ERROR(logger, "UNKOWN ERROR!");
		return TNC_RESULT_FATAL;
	}

	return TNC_RESULT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void setActivateFragmentation(int active)
{
	fragmentation.setActive(active);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int getActivateFragmentation()
{
	return fragmentation.getActive();
}
