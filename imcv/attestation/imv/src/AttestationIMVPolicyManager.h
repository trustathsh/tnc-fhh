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
 * AttestationIMVPolicyManager.
 *
 * Class for querying AttestationIMV specific entries in the policy file.
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>08.03.2010 - create class skeleton (ib)</li>
 *   <li>09.03.2010 - functionally complete (ib)</li>
 *   <li>24.03.2010 - renamed to AttestationIMVPolicyManager (awe)</li>
 * </ul>
 *
 * @date 08.03.2010
 * @author Ingo Bente (ib)
 *
 *
 */
#ifndef ATTESTATIONIMVPOLICYMANAGER_H_
#define ATTESTATIONIMVPOLICYMANAGER_H_

#include <iostream>
#include <vector> // vector
#include <stdint.h> // uint8_t
#include "attestationimvConfig.h"


namespace tncfhh {

namespace iml {

/**
 * This class represents a pcr entry from the policy.
 */
class PcrEntry {
public:
	unsigned int index;
	uint8_t value[20];
	std::string valueAsString;

	PcrEntry(unsigned int index = 0, std::string valueAsString = "0000000000000000000000000000000000000000");
	~PcrEntry();

private:
	/**
	 * Converts valueAsString to an array of byte.
	 *
	 * @param valueAsString SHA-1 hash string representation
	 */
	void stringToByteArray(const std::string &valueAsString);
};

/**
 * Enum that represents the possible values for the quoteType entry in the policy.
 */
enum QuoteType
{
	single,
	complete
};

class AttestationIMVPolicyManager
{
public:

	/**
	 * Ctor.
	 */
	AttestationIMVPolicyManager(const char *policyFile = IMV_CONFIG);

	/**
	 * Dtor.
	 */
	~AttestationIMVPolicyManager();


	/**
	 * Parses the policy file of the AttestationIMV.
	 */
	void parsePolicy(const char * policyFile);

	/**
	 * Checks if an AIK is known (i.e. properly configured in the policy file).
	 * This is done based upon the fingerprints of the AIKs.
	 *
	 * @param fingerprint - the fingerprint of the AIK that should be checked
	 *                      against the entries in the policy file.
	 */
	bool isAikKnown(std::string fingerprint);

	/**
	 * Returns the quoteType that was configured in the policy file.
	 */
	QuoteType getQuoteType();

	/**
	 * Returns a vector of PcrEntry objects. For each PCR entry in the
	 * policy file, this vector contains a corresponding object.
	 */
	std::vector<PcrEntry> getPcrEntries();

private:
	/**
	 * A list of all known AIKs.
	 * (fingerprint as std::string)
	 */
	std::vector<std::string> knownAiks;

	/**
	 * The quoteType.
	 */
	QuoteType quoteType;

	std::vector<PcrEntry> pcrEntries;

	/**
	 * Parses a line of the policy file
	 */
	void parsePolicyLine(std::string &line);
	void parsePolicyLineAik(std::string &line);
	void parsePolicyLinePcr(std::string &line);
	void parsePolicyLineQuoteType(std::string &line);
};

} // namespace iml

} // namespace tncfhh

#endif // ATTESTATIONIMVPOLICYMANAGER_H_
