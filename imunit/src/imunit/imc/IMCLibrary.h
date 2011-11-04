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
 
#ifndef TNCFHH_IML_IMCLIBRARY_H_
#define TNCFHH_IML_IMCLIBRARY_H_

#include "../IMUnitLibrary.h"
#include "AbstractIMC.h"

#include <map>

// TODO - Test it on windows.
#ifdef WIN32
#  include <windows.h>

	/**
	 * Main method for Windows DLL platform binding. Is called by LoadLibrary, FreeLibrary.
	 *
	 * @param hInstance
	 * @param fdwReason
	 * @param pvReserved
	 * @return
	 */
#	define IMCLIBRARY_OS_EXTENSION \
	int WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved) { \
		return TRUE; \
	}
#else
#	define IMCLIBRARY_OS_EXTENSION
#endif

/**
 * Initializes the TNCFHH IMCLibrary framework. Must be called
 * in the .cpp file of the concrete IMC library.
 *
 * @param name - the name of the concrete IMC library
 */
#define TNCFHH_IMCLIBRARY_INITIALIZE(name) \
	namespace tncfhh            \
    {                           \
        namespace iml           \
        {                       \
	        static name imcLib; \
        }                       \
    }							\
								\
    IMCLIBRARY_OS_EXTENSION		\
								\
    TNC_Result TNC_IMC_Initialize(TNC_IMCID imcID, TNC_Version minVersion, TNC_Version maxVersion, TNC_Version *pOutActualVersion) \
    {							\
		/* transition to C++ world */\
		return tncfhh::iml::imcLib.initialize(imcID, minVersion, maxVersion, pOutActualVersion); \
	}							\
								\
	TNC_Result TNC_IMC_ProvideBindFunction(TNC_IMCID imcID, TNC_TNCC_BindFunctionPointer bindFunction) \
    {							\
		/* transition to C++ world */\
		return tncfhh::iml::imcLib.provideBindFunction(imcID, bindFunction); \
	}							\
								\
	TNC_Result TNC_IMC_NotifyConnectionChange(TNC_IMCID imcID, TNC_ConnectionID connectionID, TNC_ConnectionState newState) \
    {							\
		/* transition to C++ world */\
		return tncfhh::iml::imcLib.notifyConnectionChange(imcID, connectionID, newState); \
	}							\
								\
	TNC_Result TNC_IMC_BeginHandshake(TNC_IMCID imcID, TNC_ConnectionID connectionID) \
    {							\
		/* transition to C++ world */\
		return tncfhh::iml::imcLib.beginHandshake(imcID, connectionID); \
	}							\
								\
	TNC_Result TNC_IMC_ReceiveMessage(TNC_IMCID imcID, TNC_ConnectionID connectionID, TNC_BufferReference messageBuffer, TNC_UInt32 messageLength, TNC_MessageType messageType) \
    {							\
		/* transition to C++ world */\
		return tncfhh::iml::imcLib.receiveMessage(imcID, connectionID, messageBuffer, messageLength, messageType); \
	}							\
								\
	TNC_Result TNC_IMC_BatchEnding(TNC_IMCID imcID, TNC_ConnectionID connectionID) \
    {							\
		/* transition to C++ world */\
		return tncfhh::iml::imcLib.batchEnding(imcID, connectionID); \
	}							\
								\
	TNC_Result TNC_IMC_Terminate(TNC_IMCID imcID) \
    {							\
		/* transition to C++ world */\
		return tncfhh::iml::imcLib.terminate(imcID); \
	}							\
/* end TNCFHH_IMCLIBRARY_INITIALIZE */

namespace tncfhh {

namespace iml {

/**
 * Class that encapsulates all state information of an IMC library.
 * Extends IMUnitLibrary. A concrete IMC/IMV library must extend this
 * class.
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>16.06.2009 - create class (ib)</li>
 * </ul>
 *
 * @date 16.06.2009
 * @author Ingo Bente (ib)
 */
class IMCLibrary: public IMUnitLibrary {
public:
	/**
	 * Ctor.
	 */
	IMCLibrary();

	/**
	 * Dtor.
	 */
	virtual ~IMCLibrary();

	/**
	 * Abstract method that must be implemented by the concrete IMC library.
	 * The implementation will normally look as follows:
	 * @code return new MyFancyIMCThatInheritsFromAbstractIMC(conID, this) @endcode
	 *
	 * @return A pointer to the new, concrete IMC instance for the given conID.
	 */
	virtual AbstractIMC *createNewImcInstance(TNC_ConnectionID conID) = 0;

	/**
	 * This function returns a pointer to a TNC client function with the
	 * specified name.
	 *
	 * @param functionName The name of the function that shall be looked up.
	 * @param tncBindFunction The bindFunction from the client that has been previously
	 *        provided by TNC_IMC_provideBindFunction.
	 *
	 * @return a pointer to the desired function.
	 */
	void * getTNCCFunctionPointer(const char *functionName,
			                     TNC_BindFunctionPointer tncBindFunction) throw (ResultException);

	/**
	 * C++ method for corresponding C-function in IFIMCImpl.cpp
	 * @see TNC_IMC_ProvideBindFunction
	 */
	TNC_Result provideBindFunction(TNC_IMCID imcID, TNC_TNCC_BindFunctionPointer bindFunction);

	/**
	 * C++ method for corresponding C-function in IFIMCImpl.cpp
	 * @see TNC_IMC_notifyConnectionChange
	 */
	TNC_Result notifyConnectionChange(TNC_IMCID imcID, TNC_ConnectionID connectionID, TNC_ConnectionState newState);

	/**
	 * C++ method for corresponding C-function in IFIMCImpl.cpp
	 * @see TNC_IMC_beginHandshake
	 */
	TNC_Result beginHandshake(TNC_IMCID imcID, TNC_ConnectionID connectionID);

	/**
	 * C++ method for corresponding C-function in IFIMCImpl.cpp
	 * @see TNC_IMC_receiveMessage
	 */
	TNC_Result receiveMessage(TNC_IMCID imcID,
			                  TNC_ConnectionID connectionID,
                              TNC_BufferReference messageBuffer,
                              TNC_UInt32 messageLength,
                              TNC_MessageType messageType);

	/**
	 * C++ method for corresponding C-function in IFIMCImpl.cpp
	 * @see TNC_IMC_batchEnding
	 */
	TNC_Result batchEnding(TNC_IMCID imcID, TNC_ConnectionID connectionID);

	/**
	 * C++ method for corresponding C-function in IFIMCImpl.cpp
	 * @see TNC_IMC_terminate
	 */
	TNC_Result terminate(TNC_IMCID imcID);

	/**
	 * This struct holds all the function pointers to the TNCC functions.
	 */
	struct TNCC_FunctionPointers{
		TNC_TNCC_ReportMessageTypesPointer reportMessageTypesPointer;
		TNC_TNCC_SendMessagePointer sendMessagePointer;
		TNC_TNCC_RequestHandshakeRetryPointer requestHandshakeRetryPointer;
		TNC_TNCC_BindFunctionPointer bindFunctionPointer;
	};

	/**
	 * The function pointer from TNC-Client
	 */
	struct TNCC_FunctionPointers tnccFunctionPointers;

protected:

	/**
	 * Checks if preconditions for provideBindFunction call are fulfilled, i.e.
	 * - the library is initialized
	 * - the imcID equals @link #imUnitLibraryID @endlink
	 * - bindFunction is not NULL
	 *
	 * @throw tncfhh::iml::ResultException if preconditions are not fulfilled
	 */
	void checkProvideBindFunction(TNC_IMCID imcID, TNC_TNCC_BindFunctionPointer bindFunction) throw (ResultException);

	/**
	 * Checks if preconditions for notifyConnectionChange call are fulfilled, i.e.
	 * - the library is initialized
	 * - the imcID equals @link #imUnitLibraryID @endlink
	 * - the new state is a valid TNC_ConnectionState
	 *
	 * @throw tncfhh::iml::ResultException if preconditions are not fulfilled
	 */
	void checkNotifyConnectionChange(TNC_IMCID imcID, TNC_ConnectionID connectionID, TNC_ConnectionState newState) throw (ResultException);

	/**
	 * Checks if preconditions for beginHandshake call are fulfilled, i.e.
	 * - the library is initialized
	 * - the imcID equals @link #imUnitLibraryID @endlink
	 *
	 * @throw tncfhh::iml::ResultException if preconditions are not fulfilled
	 */
	void checkBeginHandshake(TNC_IMCID imcID, TNC_ConnectionID connectionID) throw (ResultException);

	/**
	 * Checks if preconditions for batchEnding call are fulfilled, i.e.
	 * - the library is initialized
	 * - the imcID equals @link #imUnitLibraryID @endlink
	 *
	 * @throw tncfhh::iml::ResultException if preconditions are not fulfilled
	 */
	void checkBatchEnding(TNC_IMCID imcID, TNC_ConnectionID connectionID) throw (ResultException);

	/**
	 * Checks if preconditions for terminate call are fulfilled, i.e.
	 * - the library is initialized
	 * - the imcID equals @link #imUnitLibraryID @endlink
	 *
	 * @throw tncfhh::iml::ResultException if preconditions are not fulfilled
	 */
	void checkTerminate(TNC_IMCID imcID) throw (ResultException);

	/**
	 * Get IMC by connection id
	 *
	 * @param the connection id
	 *
	 * @return the IMC
	 *
	 * @throw tncfhh::iml::ResultException if IMC not found
	 */
	AbstractIMC * getImcInstances(TNC_ConnectionID id) throw (ResultException);

	/**
	 * Map with pointers to concrete IMC instance for a given connection ID.
	 */
	std::map<TNC_ConnectionID, AbstractIMC *> imcInstances;

};

}

}

#endif /* TNCFHH_IML_IMCLIBRARY_H_ */
