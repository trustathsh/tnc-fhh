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
 
#ifndef EXAMPLEIMC_H_
#define EXAMPLEIMC_H_

#include <imunit/imc/AbstractIMC.h>
#include "ExampleIMCLibrary.h"

/**
 * ExampleIMC.
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>19.08.2009 - create class (mbs)</li>
 * </ul>
 *
 * @date 19.08.2009
 * @author Mike Steinmetz (mbs)
 */
class ExampleIMC : public tncfhh::iml::AbstractIMC
{
public:
	/**
	 * Ctor.
	 */
	ExampleIMC(TNC_ConnectionID conID, ExampleIMCLibrary *pExampleIMCLibrary);

	/**
	 * Dtor.
	 */
	virtual ~ExampleIMC();

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

    /**
     * notifyConnectionChange
     */
    virtual TNC_Result notifyConnectionChange();
private:
    char *buf;
};

#endif /* EXAMPLEIMC_H_ */
