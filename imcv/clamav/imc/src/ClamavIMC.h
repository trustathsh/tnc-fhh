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
 
#ifndef CLAMAVIMC_H_
#define CLAMAVIMC_H_

#include <imunit/imc/AbstractIMC.h>
#include "ClamavIMCLibrary.h"
#include <string>
#include <sstream>

// this file is created by running cmake
#include <clamavimcConfig.h>

/**
 * ClamavIMC.
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>06.01.2010 - create class (ib)</li>
 * </ul>
 *
 * @date 06.01.2010
 * @author Ingo Bente (ib)
 */
class ClamavIMC: public tncfhh::iml::AbstractIMC
{
public:
	/**
	 * Ctor.
	 */
	ClamavIMC(TNC_ConnectionID conID, ClamavIMCLibrary *pClamavIMCLibrary);

	/**
	 * Dtor.
	 */
	virtual ~ClamavIMC();

	/**
	 * beginHandshake
	 */
    virtual TNC_Result beginHandshake();

    /**
     * receiveMessage
     */
    virtual TNC_Result receiveMessage(TNC_BufferReference message,
    		                          TNC_UInt32 messageLength,
    		                          TNC_MessageType messageType);

    /**
     * batchEnding
     */
    virtual TNC_Result batchEnding();
protected:

    /**
     * Reads the clamavimc.conf configuration file.
     */
    void readConfiguration();

    /**
     * Checks the operational status of Clamav. This is done by sending the
     * 'PING' command to the clamd socket. If clamd is currently running, a
     * 'PONG' is returned. The result is saved in this->resulOperationalStatus.
     */
    void checkOperationalStatus();

    /**
     * Executes the 'clamconf' command in a new process. The entire output of
     * this call is saved in this->resultClamconf.
     */
    void checkClamconf();

    /**
     * Parses the output of the check* methods and fills the this->out stream
     * accordingly.
     */
    void parseResults();

    /**
     * Controls the measurement process of the IMC. Calls
     * - readConfiguration(),
     * - checkOperationalStatus(),
     * - checkClamconf(),
     * - parseResults()
     */
    void measure();

    /**
     * Path to clamconf
     */
    std::string clamconfPath;

    /**
     * Path to clamd socket
     */
    std::string clamdSocketPath;

    /**
     * Result of operational status check
     */
    ClamavIMCLibrary::OperationalStatus resultOperationalStatus;

    /**
     * Result of clamconf execution
     */
    std::string resultClamconf;

    /**
     * Stream for the message that is sent to the IMV.
     * Content depends on the last execution of the check* methods.
     */
    std::stringstream out;

};

#endif /* CLAMAVIMC_H_ */
