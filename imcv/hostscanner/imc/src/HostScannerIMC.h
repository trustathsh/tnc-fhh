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
 
#ifndef HOSTSCANNERIMC_H_
#define HOSTSCANNERIMC_H_

#include <imunit/imc/AbstractIMC.h>
#include "HostScannerIMCLibrary.h"

/**
 * HostScannerIMC
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>06.05.2006 - create class (Daniel Wuttke)</li>
 *   <li>09.05.2008 - completely redesign (mbs)</li>
 *   <li>19.06.2009 - redesign 0.6.0 (ib)</li>
 *   <li>30.07.2009 - update to 0.6.0 (mbs)</li>
 * </ul>
 *
 * @class HostScannerIMC
 * @date 06.05.2006
 * @author Mike Steinmetz (mbs)
 * @author Daniel Wuttke
 */
class HostScannerIMC : public tncfhh::iml::AbstractIMC
{
private:
	/**
	 * Layer 4 Protocols
	 */
	enum Protocol {TCP, UDP};

	/**
	 * Status of a Port
	 */
	enum Status {open, close};

	/**
	 * Policy is a triple of protocol, port and status
	 */
	class Policy
	{
		public:
			/**
			 * The layer 4 protocol
			 */
			Protocol protocol;

			/**
			 * The port number
			 */
			unsigned short port;

			/**
			 * Status of the port
			 */
			Status status;

			/**
			 * Ctor
			 */
			Policy(Protocol pr, unsigned short po, Status s);
	};

	bool readLine(std::istream *in, char *buf, const int size);

public:
	/**
	 * Ctor
	 */
    HostScannerIMC(TNC_ConnectionID cid, HostScannerIMCLibrary *pHostScannerIMCLibrary);

    /**
     * Dtor
     */
    virtual ~HostScannerIMC();

    /**
     * beginHandshake
     */
    virtual TNC_Result beginHandshake();

    /**
     * receiveMessage
     */
    virtual TNC_Result receiveMessage(TNC_BufferReference message, TNC_UInt32 messageLength, TNC_MessageType messageType);
};

#endif /*HostScannerIMC_H_*/
