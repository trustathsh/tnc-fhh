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
 
#ifndef TNCFHH_IMUNITLIBRARY_H_
#define TNCFHH_IMUNITLIBRARY_H_

#include "exception/ResultException.h"

#include <tcg/tnc/tncifimc.h>
#include <tcg/tnc/tncifimv.h>
#include <tcg/tnc/fhhaddon.h>
#include <string>

#include <tncfhhConfig.h> 

namespace tncfhh {

namespace iml {

/**
 * IMUnit ID is the generalization of IMC/IMV ID.
 */
typedef unsigned long TNC_IMUnitID;

/**
 * Function pointer to bind function.
 */
typedef TNC_Result (*TNC_BindFunctionPointer)(TNC_IMUnitID id,
		char *functionName, void **pOutfunctionPointer);

/**
 * Class that encapsulates all state information of an IMC/IMV library.
 * There is one instance of this class per IMC/IMV library. Known subclasses
 * are IMCLibrary and IMVLibrary.
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>07.02.2008 - create class (mbs)</li>
 *   <li>06.01.2009 - added comment (ib)</li>
 *   <li>11.06.2009 - tnc_fhh 0.6.0 redesign (ib,mbs)</li>
 * </ul>
 *
 * @date 07.02.2008
 * @author Mike Steinmetz (mbs)
 * @author Ingo Bente (ib)
 */
class IMUnitLibrary {

public:

	/**
	 * Ctor.
	 */
	IMUnitLibrary();

	/**
	 * Dtor.
	 */
	virtual ~IMUnitLibrary();

	/**
	 * Getter for @link #messageTypeList @endlink
	 *
	 * @return @link #messageTypeList @endlink
	 */
	TNC_MessageTypeList getMessageTypeList();

	/**
	 * Getter for @link #typeCount @endlink
	 *
	 * @return @link #typeCount @endlink
	 */
	TNC_UInt32 getMessageTypeCount();

	/**
	 * Getter for @link #imUnitLibraryName @endlink
	 *
	 * @return @link #imUnitLibraryName @endlink
	 */
	std::string getIMUnitLibraryName();

	/**
	 * Getter for @link #isIMUnitLibraryInitialized @endlink
	 *
	 * @return @link #isIMUnitLibraryInitialized @endlink
	 */
	bool getIMUnitLibraryInitialized();

	/**
	 * Getter for @link #imUnitLibraryID @endlink
	 *
	 * @return @link #imUnitLibraryID @endlink
	 */
	TNC_IMUnitID getIMUnitLibraryID();

	/**
	 * Getter for @link #ifImcImvVersion @endlink
	 *
	 * @return @link #ifImcImvVersion @endlink
	 */
	TNC_Version getIfImcImvVersion();

	/**
	 * Setter for @link imUnitLibraryName @endlink
	 */
	void setIMUnitLibraryName(std::string &imUnitName);

	/**
	 * Setter for @link #isIMUnitLibraryInitialized @endlink
	 */
	void setIMUnitLibraryInitialized(bool initialized);

	/**
	 * Setter for @link #imUnitLibraryID @endlink
	 */
	void setIMUnitLibraryID(TNC_IMUnitID id);

	/**
	 * Setter for @link #ifImcImvVersion @endlink
	 */
	void setIfImcImvVersion(TNC_Version version);

	/**
	 * Initializes the IMUnit. Called by TNC_IMC/V_Initialize.
	 *
	 * @param imUnitID - The ID of the IMC/VLibrary assigned by the TNCC/S.
	 * @param minVersion - minimum Version of IF-IMC/V supported by TNCC/S.
	 * @param maxVersion - maximum Version of IF-IMC/V supported by TNCC/S.
	 * @param pOutActualVersion - Version of IF-IMC/V used for communication
	 *                            between the TNCC/S and this library.
	 */
	TNC_Result initialize(TNC_IMUnitID imUnitID, // in
			TNC_Version minVersion, // in
			TNC_Version maxVersion, // in
			TNC_Version *pOutActualVersion // out
			);


protected:

	/**
	 * Add messageType in @link #messageTypeList @endlink
	 *
	 * @param vendorId VendorID (last 24 Bits in messageType)
	 * @param messageSubtype MessageSubtype (first 8 Bits in messageType)
	 */
	void addMessageType(TNC_VendorID vendorId, TNC_MessageSubtype messageSubtype);

	/**
	 * Add messageType in @link #messageTypeList @endlink
	 *
	 * @param messageType MessageType
	 */
	void addMessageType(TNC_MessageType messageType);

	/**
	 * Checks if the Log4cxx Framework is initialized.
	 */
    bool checkLog4cxxInitialized();

	/**
	 * Checks if initialize call for library is correct.
	 */
	void checkInitialize(TNC_IMUnitID imUnitID, TNC_Version minVersion,
			TNC_Version maxVersion, TNC_Version *pOutActualVersion)
			throw (ResultException);
	/**
	 * Checks that IMUnitLibrary is initialized. Throws an exception if
	 * IMUnitLibrary is not initialized.
	 */
	void checkIMUnitLibraryIsInitialized() throw (ResultException);

	/**
	 * Checks if imUnitID matches the libraries ID.
	 */
	void checkIMUnitID(TNC_IMUnitID imUnitID)
			throw (ResultException);

	/**
	 * Checks if receiveMessage preconditions are fulfilled.
	 *
	 * @throw tncfhh::iml::ResultException if preconditions are not fulfilled
	 */
	void checkReceiveMessage(TNC_IMUnitID id,
			TNC_BufferReference messageBuffer, TNC_UInt32 messageLength,
			TNC_MessageType messageType) throw (ResultException);

	/**
	 * List of message types for which the IMC/IMV wishes to receive messages.
	 * @see section 3.8.1 TNC_TNCS_ReportMessageTypes(...) in IF-IMV version 1.2
	 *      http://www.trustedcomputinggroup.org/resources/tnc_ifimv_specification_version_12
	 * @see section 3.8.1 TNC_TNCC_ReportMessageTypes(...) in IF-IMC version 1.2
	 *      http://www.trustedcomputinggroup.org/resources/tnc_ifimc_specification_version_12
	 */
	TNC_MessageTypeList messageTypeList;

	/**
	 * Number of message types for which the IMC/IMV wishes to receive messages.
	 * @see section 3.8.1 TNC_TNCS_ReportMessageTypes(...) in IF-IMV version 1.2
	 *      http://www.trustedcomputinggroup.org/resources/tnc_ifimv_specification_version_12
	 * @see section 3.8.1 TNC_TNCS_ReportMessageTypes(...) in IF-IMC version 1.2
	 *      http://www.trustedcomputinggroup.org/resources/tnc_ifimc_specification_version_12
	 */
	TNC_UInt32 typeCount;

	/**
	 * The name of the IMC/IMV library. This name is used for logging purposes.
	 */
	std::string imUnitLibraryName;

	/**
	 * Flag indicating if IMC/IMV library is initialized
	 */
	bool isIMUnitLibraryInitialized;

	/**
	 * ID of the IMC/IMV library assigned by the TNCC/TNCS
	 */
	TNC_IMUnitID imUnitLibraryID;

	/**
	 * IF-IMC/V Version used between TNCC/IMC or TNCS/IMV.
	 */
	TNC_Version ifImcImvVersion;
};

} // namespace iml

} // namespace tncfhh

#endif /*TNCFHH_IMUNITLIBRARY_H_*/
