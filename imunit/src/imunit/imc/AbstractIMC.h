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
 
#ifndef TNCFHH_IML_ABSTRACTIMC_H_
#define TNCFHH_IML_ABSTRACTIMC_H_

#include "../AbstractIMUnit.h"
#include "TNCC.h"

namespace tncfhh {

namespace iml {

// Prototype instead of include IMCLibrary.h
class IMCLibrary;

/**
 * AbstractIMC represents the abstract super class for any concrete IMC implementation. It manages
 * the current connectionState and provides helper functions for parsing message types. Furthermore,
 * it declares two abstract methods (begin and receive) which must be implemented by derived classes.
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>09.04.2006 - create class (Daniel Wuttke)</li>
 *   <li>09.02.2008 - add tncc (mbs)</li>
 *   <li>08.05.2008 - change namespace (mbs)</li>
 *   <li>16.06.2009 - redesign 0.6.0 (ib)</li>
 * </ul>
 *
 * @class AbstractIMC
 * @brief Represents the abstract super class for any concrete IMC implementation.
 * @date 09.04.2006
 * @author Daniel Wuttke
 * @author Ingo Bente
 * @author Mike Steinmetz (mbs)
 */
class AbstractIMC : public AbstractIMUnit
{
    private:

    protected:

    	/**
    	 * The TNC part of the IF-IMC interface provided by an instance of TNCC.
    	 * The IMC calls the TNCC <b>only</b> via this attribute.
    	 */
    	TNCC tncc;

    public:

    	/**
    	 * Ctor.
    	 *
    	 * @param conID The TNC_ConnectionID for the IMC instance.
    	 * @param pImcLibrary Pointer to an instance of the concrete IMC library.
    	 */
        AbstractIMC(TNC_ConnectionID conID, IMCLibrary *pImcLibrary);

        /**
         * Dtor.
         */
        virtual ~AbstractIMC();

        /**
         * Invoked after TNC_IMC_BeginHandshake() has been called by TNC client. Has to be implemented
         * by concrete IMC. This method is called to trigger a TNC handshake.
         *
         * @param message The message that shall be sent from IMC to IMV.
         * @param messageType The type of the message
         * @param messageLength The length of the message
         * @return Result code (specified in tncifimc.h) that indicates if the operation was succesful or not.
         */
        virtual TNC_Result beginHandshake() = 0;
};

}  // namespace iml

}  // namespace tncfhh

#endif /*TNCFHH_IML_ABSTRACTIMC_H_*/
