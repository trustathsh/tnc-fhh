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
 
#ifndef TNCFHH_IML_IMVLIBRARY_H_
#define TNCFHH_IML_IMVLIBRARY_H_

#include "../IMUnitLibrary.h"
#include "AbstractIMV.h"

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
 * Initializes the TNCFHH IMVLibrary framework. Must be called
 * in the .cpp file of the concrete IMV library.
 *
 * @param name - the name of the concrete IMV library
 */
#define TNCFHH_IMVLIBRARY_INITIALIZE(name) \
	namespace tncfhh            \
    {                           \
        namespace iml           \
        {                       \
	        static name imvLib; \
        }                       \
    }							\
								\
    IMCLIBRARY_OS_EXTENSION		\
								\
	TNC_Result TNC_IMV_Initialize(TNC_IMVID imvID, TNC_Version minVersion, TNC_Version maxVersion, TNC_Version *pOutActualVersion) \
	{							\
		/* transition to C++ world */\
		return tncfhh::iml::imvLib.initialize(imvID, minVersion, maxVersion, pOutActualVersion); \
	}							\
	TNC_Result TNC_IMV_ProvideBindFunction(TNC_IMVID imvID, TNC_TNCS_BindFunctionPointer bindFunction) \
	{							\
		/* transition to C++ world */\
		return tncfhh::iml::imvLib.provideBindFunction(imvID, bindFunction); \
	}							\
	TNC_Result TNC_IMV_NotifyConnectionChange(TNC_IMVID imvID, TNC_ConnectionID connectionID, TNC_ConnectionState newState) \
	{							\
		/* transition to C++ world */\
		return tncfhh::iml::imvLib.notifyConnectionChange(imvID, connectionID, newState); \
	}							\
	TNC_Result TNC_IMV_SolicitRecommendation(TNC_IMVID imvID, TNC_ConnectionID connectionID) \
	{							\
		/* transition to C++ world */\
		return tncfhh::iml::imvLib.solicitRecommendation(imvID, connectionID); \
	}							\
	TNC_Result TNC_IMV_ReceiveMessage(TNC_IMVID imvID, TNC_ConnectionID connectionID, TNC_BufferReference messageBuffer, TNC_UInt32 messageLength, TNC_MessageType messageType) \
	{							\
		/* transition to C++ world */\
		return tncfhh::iml::imvLib.receiveMessage(imvID, connectionID, messageBuffer, messageLength, messageType); \
	}							\
	TNC_Result TNC_IMV_BatchEnding(TNC_IMVID imvID, TNC_ConnectionID connectionID) \
	{							\
		/* transition to C++ world */\
		return tncfhh::iml::imvLib.batchEnding(imvID, connectionID); \
	}							\
	TNC_Result TNC_IMV_Terminate(TNC_IMVID imvID) \
	{							\
		/* transition to C++ world */\
		return tncfhh::iml::imvLib.terminate(imvID); \
	}							\
/* end TNCFHH_IMVLIBRARY_INITIALIZE */

namespace tncfhh {

namespace iml {

// Version number of supported TNC IF-IMV specification
#define TNC_IFIMV_VERSION TNC_IFIMV_VERSION_1

/**
 * Class that encapsulates all state information of an IMV library.
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>16.06.2009 - create class (ib)</li>
 * </ul>
 *
 * @date 16.06.2009
 * @author Ingo Bente (ib)
 */
class IMVLibrary: public IMUnitLibrary {
public:
	/**
	 * Ctor.
	 */
	IMVLibrary();

	/**
	 * Dtor.
	 */
	virtual ~IMVLibrary();

	/**
	 * Abstract method that must be implemented by the concrete IMV library.
	 * The implementation will normally look as follows:
	 * @code return new MyFancyIMVThatInheritsFromAbstractIMV(conID, this) @endcode
	 *
	 * @return A pointer to the new, concrete IMV instance for the given conID.
	 */
	virtual AbstractIMV *createNewImvInstance(TNC_ConnectionID conID) = 0;

	/**
	 * Get IMV by connection id
	 *
	 * @param the connection id
	 *
	 * @return the IMV
	 *
	 * @throw tncfhh::iml::ResultException if IMV not found
	 */
	AbstractIMV * getImvInstances(TNC_ConnectionID id) throw (ResultException);

	/**
	 * This function returns a pointer to a TNC server function with the
	 * specified name.
	 *
	 * @param functionName The name of the function that shall be looked up.
	 * @param tncBindFunction The bindFunction from the server that has been previously
	 *        provided by TNC_IMV_provideBindFunction.
	 *
	 * @return a pointer to the desired function.
	 */
	void * getTNCSFunctionPointer(const char *functionName,
			                     TNC_BindFunctionPointer tncBindFunction) throw (ResultException);
	/**
	 * C++ method for corresponding C-function in IFIMVImpl.cpp
	 * @see TNC_IMV_ProvideBindFunction
	 */
	TNC_Result provideBindFunction(TNC_IMVID imvID, TNC_TNCS_BindFunctionPointer bindFunction);

	/**
	 * C++ method for corresponding C-function in IFIMVImpl.cpp
	 * @see TNC_IMV_notifyConnectionChange
	 */
	TNC_Result notifyConnectionChange(TNC_IMVID imvID, TNC_ConnectionID connectionID, TNC_ConnectionState newState);

	/**
	 * C++ method for corresponding C-function in IFIMVImpl.cpp
	 * @see TNC_IMV_solicitRecommendation
	 */
	TNC_Result solicitRecommendation(TNC_IMVID imvID, TNC_ConnectionID connectionID);

	/**
	 * C++ method for corresponding C-function in IFIMVImpl.cpp
	 * @see TNC_IMV_receiveMessage
	 */
	TNC_Result receiveMessage(TNC_IMVID imvID,
			                  TNC_ConnectionID connectionID,
                              TNC_BufferReference messageBuffer,
                              TNC_UInt32 messageLength,
                              TNC_MessageType messageType);

	/**
	 * C++ method for corresponding C-function in IFIMVImpl.cpp
	 * @see TNC_IMV_batchEnding
	 */
	TNC_Result batchEnding(TNC_IMVID imvID, TNC_ConnectionID connectionID);

	/**
	 * C++ method for corresponding C-function in IFIMVImpl.cpp
	 * @see TNC_IMV_terminate
	 */
	TNC_Result terminate(TNC_IMVID imvID);

	/**
	 * This struct holds all the function pointers to the TNCS functions.
	 */
	struct TNCS_FunctionPointers{
		TNC_TNCS_ReportMessageTypesPointer reportMessageTypesPointer;
		TNC_TNCS_SendMessagePointer sendMessagePointer;
		TNC_TNCS_RequestHandshakeRetryPointer requestHandshakeRetryPointer;
		TNC_TNCS_ProvideRecommendationPointer provideRecommendationPointer;
		TNC_TNCS_BindFunctionPointer bindFunctionPointer;
		TNC_TNCS_GetAttributePointer getAttributePointer;
		TNC_TNCS_SetAttributePointer setAttributePointer;
	};

	/**
	 * The function pointer from TNC-Server
	 */
	struct TNCS_FunctionPointers tncsFunctionPointers;

protected:
	/**
	 * Checks if preconditions for provideBindFunction call are fulfilled, i.e.
	 * - the library is initialized
	 * - the imvID equals @link #imUnitLibraryID @endlink
	 * - bindFunction is not NULL
	 *
	 * @throw tncfhh::iml::ResultException if preconditions are not fulfilled
	 */
	void checkProvideBindFunction(TNC_IMVID imvID, TNC_TNCS_BindFunctionPointer bindFunction) throw (ResultException);

	/**
	 * Checks if preconditions for notifyConnectionChange call are fulfilled, i.e.
	 * - the library is initialized
	 * - the imvID equals @link #imUnitLibraryID @endlink
	 * - the new state is a valid TNC_ConnectionState
	 *
	 * @throw tncfhh::iml::ResultException if preconditions are not fulfilled
	 */
	void checkNotifyConnectionChange(TNC_IMVID imvID, TNC_ConnectionID connectionID, TNC_ConnectionState newState) throw (ResultException);

	/**
	 * Checks if preconditions for solicitRecommendation call are fulfilled, i.e.
	 * - the library is initialized
	 * - the imvID equals @link #imUnitLibraryID @endlink
	 *
	 * @throw tncfhh::iml::ResultException if preconditions are not fulfilled
	 */
	void checkSolicitRecommendation(TNC_IMVID imvID, TNC_ConnectionID connectionID) throw (ResultException);

	/**
	 * Checks if preconditions for batchEnding call are fulfilled, i.e.
	 * - the library is initialized
	 * - the imvID equals @link #imUnitLibraryID @endlink
	 *
	 * @throw tncfhh::iml::ResultException if preconditions are not fulfilled
	 */
	void checkBatchEnding(TNC_IMVID imvID, TNC_ConnectionID connectionID) throw (ResultException);

	/**
	 * Checks if preconditions for terminate call are fulfilled, i.e.
	 * - the library is initialized
	 * - the imvID equals @link #imUnitLibraryID @endlink
	 *
	 * @throw tncfhh::iml::ResultException if preconditions are not fulfilled
	 */
	void checkTerminate(TNC_IMVID imvID) throw (ResultException);

	/**
	 * Map with pointers to concrete IMV instance for a given connection ID.
	 */
	std::map<TNC_ConnectionID, AbstractIMV *> imvInstances;
};

} // namespace iml

} // namespace tncfhh

#endif /* TNCFHH_IML_IMVLIBRARY_H_ */
