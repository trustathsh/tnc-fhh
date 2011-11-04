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
 
#ifndef DUMMYIMC_H_
#define DUMMYIMC_H_

#include <imunit/imc/AbstractIMC.h>
#include "DummyIMCLibrary.h"

// this file is created by running cmake
#include <dummyimcConfig.h>

/**
 * DummyIMC.
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>12.01.2009 - create class (ib)</li>
 *   <li>19.06.2009 - redesign 0.6.0 (ib)</li>
 * </ul>
 *
 * @date 12.01.2009
 * @author Ingo Bente (ib)
 */
class DummyIMC: public tncfhh::iml::AbstractIMC
{
public:
	/**
	 * Ctor.
	 */
	DummyIMC(TNC_ConnectionID conID, DummyIMCLibrary *pDummyIMCLibrary);

	/**
	 * Dtor.
	 */
	virtual ~DummyIMC();

	/**
	 * beginHandshake
	 */
    virtual TNC_Result beginHandshake();

    /**
     * receive Message
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
	 * value read from DUMMY_IMC_FILE
	 */
	std::string localFileAction;

	/**
	 * Counter for number of messages send to IMV.
	 */
	unsigned int count;
};

#endif /* DUMMYIMC_H_ */
