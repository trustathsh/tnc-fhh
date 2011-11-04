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
 
#include "ClamavIMV.h"

#include <imunit/exception/ResultException.h>

#include <tncxacml/XACMLHelper.h>
#include <tncxacml/XACMLConstants.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h> // strlen()
#include <algorithm> // std::remove
#include <log4cxx/logger.h>

#include <boost/algorithm/string.hpp>

using namespace log4cxx;

static LoggerPtr logger(Logger::getLogger(
		"IMUnit.AbstractIMUnit.AbstractIMV.ClamavIMV"));

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
ClamavIMV::ClamavIMV(TNC_ConnectionID conID,
		ClamavIMVLibrary *pClamavImvLibrary) :
	tncfhh::iml::AbstractIMV(conID, pClamavImvLibrary) {
	//
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
ClamavIMV::~ClamavIMV() {
	//
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result ClamavIMV::notifyConnectionChange() {
	LOG4CXX_TRACE(logger, "notifyConnectionChange");
	// check that TNC handshake has started
	if (this->getConnectionState() == TNC_CONNECTION_STATE_HANDSHAKE) {
		// parse policy
		this->parsePolicy();
	}
	return TNC_RESULT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result ClamavIMV::receiveMessage(TNC_BufferReference message,
		TNC_UInt32 messageLength, TNC_MessageType messageType) {
	std::stringstream inSS; // stream for incoming message
	std::string line; // buffer for each line of incoming message
	LOG4CXX_TRACE(logger, "receiveMessage");

	// save incoming message to stream
	inSS.write((const char*)message, messageLength);

	std::vector<Measurement>* measurements = new std::vector<Measurement>;

	LOG4CXX_DEBUG(logger, "Parsing message from IMC.");
	// read all lines of message
	while(std::getline(inSS, line)){
		// parse incoming line
		Key inKey;
		std::string inValue;
		std::string inKeyAsString;
		int pos1, pos2;
		// check what line we have here
		if(line.compare(0, strlen(CLAMAVIMV_CONF_VERSION), CLAMAVIMV_CONF_VERSION) == 0){
			// version line
			LOG4CXX_TRACE(logger, "Found version number.");
			inKey = Version;
			inKeyAsString = CLAMAVIMV_CONF_VERSION;
			inValue = line.substr(strlen(CLAMAVIMV_CONF_VERSION) + 2); // +2 for skipping ': '
			measurements->push_back(Measurement(inKey, inKeyAsString, inValue));
		} else if (line.compare(0, strlen(CLAMAVIMV_CONF_OPERATIONAL_STATUS), CLAMAVIMV_CONF_OPERATIONAL_STATUS) == 0){
			// operational status line
			LOG4CXX_TRACE(logger, "Found operational status.");
			inKey = OperationalStatus;
			inKeyAsString = CLAMAVIMV_CONF_OPERATIONAL_STATUS;
			inValue = line.substr(strlen(CLAMAVIMV_CONF_OPERATIONAL_STATUS) + 1); // +1 for skipping ' '
			measurements->push_back(Measurement(inKey, inKeyAsString, inValue));
		} else if (line.compare(0, strlen(CLAMAVIMV_CONF_MAIN_VERSION), CLAMAVIMV_CONF_MAIN_VERSION) == 0){
			// main.cvd line
			LOG4CXX_TRACE(logger, "Found main.cvd-version.");
			inKey = Main_Version;
			inKeyAsString = CLAMAVIMV_CONF_MAIN_VERSION;
			inValue = line.substr(strlen(CLAMAVIMV_CONF_MAIN_VERSION) + 6); // +6 for skipping '.cld: ' or 'cvd: '
			pos1 = inValue.find_first_of(' '); // space before version number
			pos2 = inValue.find_first_of(',', pos1); // comma after version number
			inValue = inValue.substr(pos1+1, pos2-pos1-1);
			measurements->push_back(Measurement(inKey, inKeyAsString, inValue));
		} else if (line.compare(0, strlen(CLAMAVIMV_CONF_DAILY_VERSION), CLAMAVIMV_CONF_DAILY_VERSION) == 0){
			// daily.cvd line
			LOG4CXX_TRACE(logger, "Found daily.cvd-version.");
			inKey = Daily_Version;
			inKeyAsString = CLAMAVIMV_CONF_DAILY_VERSION;
			inValue = line.substr(strlen(CLAMAVIMV_CONF_DAILY_VERSION) + 6); // +6 for skipping '.cld: ' or 'cvd: '
			pos1 = inValue.find_first_of(' '); // space before version number
			pos2 = inValue.find_first_of(',', pos1); // comma after version number
			inValue = inValue.substr(pos1+1, pos2-pos1-1);
			measurements->push_back(Measurement(inKey, inKeyAsString, inValue));
		} else {
			LOG4CXX_TRACE(logger, "Invalid message from IMC: " << line);
		}
	}

	// try XACML evaluation
	bool xacmlSuccessful = doXACMLEvaluation(measurements);
	if (xacmlSuccessful) {
		LOG4CXX_DEBUG(logger, "Evaluation by XACML was successful.");
		return TNC_RESULT_SUCCESS;
	} else {
		// do local evaluation
		LOG4CXX_DEBUG(logger, "Evaluation by XACML was not successful, trying local policy checks.");
		return doLocalEvaluation(measurements);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result ClamavIMV::batchEnding() {
	LOG4CXX_TRACE(logger, "batchEnding");
	return TNC_RESULT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ClamavIMV::parsePolicy() {
	LOG4CXX_DEBUG(logger, "Reading policy from " << IMV_CONFIG);
        this->policyEntries.clear();
	std::ifstream file(IMV_CONFIG);
	std::string line;
	std::string key;
	std::string operant;
	std::string value;
	int pos0, pos1, pos2, pos3;

	// read file
	while (std::getline(file, line)) {
		// skip empty lines
		if (line.length() == 0)
			continue;
		// skip comments
		if (line[0] == '#')
			continue;

		// find first no space (valid) character of line
		pos0 = line.find_first_not_of(' ', 0);
		// find first space after that valid character (== end of key)
		pos1 = line.find_first_of(' ', pos0);
		// find the next valid character ( == start of operant)
		pos2 = line.find_first_not_of(' ', pos1);
		// find first space after that valid character (== end of operant)
		pos3 = line.find_first_of(' ', pos2);

		// get key
		key = line.substr(pos0, pos1 - pos0);
		// get operant
		operant = line.substr(pos2, pos3 - pos2);
		// get value
		value = line.substr(pos3);
		// remove spaces
		value.erase(std::remove(value.begin(), value.end(), ' '), value.end());

		LOG4CXX_TRACE(logger, key << " " << operant << " " << value);

		// save entry
		this->savePolicyEntry(key, operant, value);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ClamavIMV::savePolicyEntry(std::string key, std::string operant, std::string value) {
	Key tmpKey;
	Operant tmpOperant;

	// map key to enum
	if (key.compare(CLAMAVIMV_CONF_OPERATIONAL_STATUS) == 0) {
		tmpKey = OperationalStatus;
	} else if (key.compare(CLAMAVIMV_CONF_VERSION) == 0) {
		tmpKey = Version;
	} else if (key.compare(CLAMAVIMV_CONF_MAIN_VERSION) == 0) {
		tmpKey = Main_Version;
	} else if (key.compare(CLAMAVIMV_CONF_DAILY_VERSION) == 0) {
		tmpKey = Daily_Version;
	} else {
		LOG4CXX_WARN(logger, "Invalid key in policy " << key);
		return;
	}

	// map operant to enum
	if (operant.compare("=") == 0) {
		tmpOperant = eq;
	} else if (operant.compare(">") == 0) {
		tmpOperant = gt;
	} else if (operant.compare("<") == 0) {
		tmpOperant = lt;
	} else if (operant.compare(">=") == 0) {
		tmpOperant = gte;
	} else if (operant.compare("<=") == 0) {
		tmpOperant = lte;
	} else if (operant.compare("!=") == 0) {
		tmpOperant = uneq;
	} else {
		LOG4CXX_WARN(logger, "Invalid operant in policy " << operant);
		return;
	}

	// ckeck for invalid policy entries
	if (tmpKey == OperationalStatus && (tmpOperant != eq && tmpOperant != uneq)) {
		LOG4CXX_WARN(logger, "Invalid key operant combination " << key << " " << operant);
		return;
	}

	// valid entry
	this->policyEntries.push_back(PolicyEntry(tmpKey ,tmpOperant, value));
}

ClamavIMV::PolicyEntry::PolicyEntry(Key key, Operant operant, std::string value) :
	key(key), operant(operant), value(value) {
	// ...
}

ClamavIMV::Measurement::Measurement(Key key, std::string keyAsString, std::string value) :
	key(key), keyAsString(keyAsString), value(value)  {
	// ...
}

TNC_IMV_Action_Recommendation ClamavIMV::doLocalEvaluation(std::vector<Measurement>* measurements) {
	// result of policy check
	bool result = false;
	std::string operantAsString;
	// check each line against matching policy entry
	for (std::vector<PolicyEntry>::iterator iter = this->policyEntries.begin();
			iter != this->policyEntries.end();
			++iter) {
		for (std::vector<Measurement>::iterator mIter = measurements->begin(); mIter != measurements->end(); ++mIter) {
			// check if keys match
			if(iter->key == mIter->key){
				std::string inValue = mIter->value;
				if (iter->key == OperationalStatus){
					inValue.erase(std::remove(inValue.begin(), inValue.end(), '.'), inValue.end());
				}

				// integer values (for incoming and policy)
				int in, policy;
				// streams for incoming and policy value
				std::stringstream inSS(inValue), policySS(iter->value);
				// try to wrap incoming values to decimal numbers
				// if this fails, it is not really a big deal
				// (i.e. when operational status is received)
				inSS >> in; policySS >> policy;

				// Checking measurement against policy entry according to operant
				if(iter->operant == eq){
					operantAsString = "==";
					result = (iter->value.compare(inValue) == 0);
				}
				if(iter->operant == uneq){
					operantAsString = "!=";
					result = (iter->value.compare(inValue) != 0);
				}

				if(iter->operant == gt){
					operantAsString = ">";
					result = in > policy;
				}
				if(iter->operant == lt){
					operantAsString = "<";
					result = in < policy;
				}
				if(iter->operant == gte){
					operantAsString = ">=";
					result = in >= policy;
				}
				if(iter->operant == lte){
					operantAsString = "<=";
					result = in <= policy;
				}

				if(iter->key != OperationalStatus) {
					result = compareVersionStrings(inValue, iter->value);
				}

				if(result){
					LOG4CXX_DEBUG(logger, "Policy check OK: " << mIter->keyAsString << " " << inValue << " " << operantAsString << " " << iter->value);
				}
				else {
					// a policy check has failed. provide 'ISOLATE' recommendation
					LOG4CXX_INFO(logger, "Policy check FAILED: " << mIter->keyAsString << " " << inValue << " " << operantAsString << " " << iter->value);
					LOG4CXX_INFO(logger, "Providing 'ISOLATE' recommendation");
					this->actionRecommendation = TNC_IMV_ACTION_RECOMMENDATION_ISOLATE;
					this->evaluationResult = TNC_IMV_EVALUATION_RESULT_NONCOMPLIANT_MAJOR;
					this->validationFinished = true;
					this->tncs.provideRecommendation(this->actionRecommendation, this->evaluationResult);
					return TNC_RESULT_SUCCESS;
				}
			}
		}
	}

	// all policy checks were OK. provide 'ALLOW' recommendation
	LOG4CXX_DEBUG(logger, "All policy checks OK. Providing 'ALLOW' recommendation.");
	this->actionRecommendation = TNC_IMV_ACTION_RECOMMENDATION_ALLOW;
	this->evaluationResult = TNC_IMV_EVALUATION_RESULT_COMPLIANT;
	this->validationFinished = true;
	this->tncs.provideRecommendation(this->actionRecommendation, this->evaluationResult);
	return TNC_RESULT_SUCCESS;
}

bool ClamavIMV::compareVersionStrings(std::string in, std::string policy)
{
    std::vector<std::string> inNumbers;
    std::vector<std::string> policyNumbers;
    boost::split(inNumbers, in, boost::is_any_of("."));
    boost::split(policyNumbers, policy, boost::is_any_of("."));

    std::vector<std::string>::iterator inValue = inNumbers.begin();
    std::vector<std::string>::iterator policyValue = policyNumbers.begin();
    for (; (inValue != inNumbers.end()) && (policyValue != policyNumbers.end());
            inValue++, policyValue++) {
        LOG4CXX_DEBUG(logger, "Version comparison: " << *inValue << " >= " << *policyValue);
        if (*inValue < *policyValue) {
            LOG4CXX_DEBUG(logger, "Policy not fullfilled");
            return false;
        }
    }

    LOG4CXX_DEBUG(logger, "Policy fullfilled");
    return true;
}


bool ClamavIMV::doXACMLEvaluation(std::vector<Measurement>* measurements) {
	TNC_UInt32 usernameBufferLength = 128;
	TNC_BufferReference usernameBuffer = new TNC_Buffer[usernameBufferLength];
	TNC_UInt32* pOutValueLength = (TNC_UInt32*) malloc(sizeof(TNC_UInt32));

	try {
		bool result = this->tncs.getAttribute(TNC_ATTRIBUTEID_USERNAME, 128, usernameBuffer, pOutValueLength);

		/**
		 * if result is true, the TNCS provides the getAttribute-method
		 */
		if (result) {
			LOG4CXX_DEBUG(logger, "Username for this connection: " << usernameBuffer);

			/**
			 * Create the XACML-Request
			 */
			DOMDocument* request = tncfhh::iml::XACMLHelper::createRequest();

			DOMElement* subjectUsername = tncfhh::iml::XACMLHelper::createSubject(request);
			DOMElement* attributeUsername = tncfhh::iml::XACMLHelper::createAttribute(request, tncfhh::iml::XACMLConstants::attribute_id_subject_id, tncfhh::iml::XACMLConstants::data_type_xml_string, usernameBuffer);
			tncfhh::iml::XACMLHelper::addAttributeToSubject(subjectUsername, attributeUsername);
			tncfhh::iml::XACMLHelper::addSubjectToRequest(request, subjectUsername);

			DOMElement* subjectIFMComponentVendorID = tncfhh::iml::XACMLHelper::createSubject(request, tncfhh::iml::XACMLConstants::subject_category_tnc_ifm_component);
			DOMElement* attributeVendorID = tncfhh::iml::XACMLHelper::createAttribute(request, tncfhh::iml::XACMLConstants::attribute_id_component_vendor_id, tncfhh::iml::XACMLConstants::data_type_xml_string, "0x000000");
			tncfhh::iml::XACMLHelper::addAttributeToSubject(subjectIFMComponentVendorID, attributeVendorID);
			tncfhh::iml::XACMLHelper::addSubjectToRequest(request, subjectIFMComponentVendorID);

			DOMElement* subjectIFMComponentSubType = tncfhh::iml::XACMLHelper::createSubject(request, tncfhh::iml::XACMLConstants::subject_category_tnc_ifm_component);
			DOMElement* attributeSubtype = tncfhh::iml::XACMLHelper::createAttribute(request, tncfhh::iml::XACMLConstants::attribute_id_component_subtype, tncfhh::iml::XACMLConstants::data_type_xml_string, "2");
			tncfhh::iml::XACMLHelper::addAttributeToSubject(subjectIFMComponentSubType, attributeSubtype);
			tncfhh::iml::XACMLHelper::addSubjectToRequest(request, subjectIFMComponentSubType);

			DOMElement* subjectProductInformation = tncfhh::iml::XACMLHelper::createSubject(request, tncfhh::iml::XACMLConstants::subject_category_tnc_ifm_attribute_product_information);
			DOMElement* attributeProductInformationAttributeVendorID = tncfhh::iml::XACMLHelper::createAttribute(request, tncfhh::iml::XACMLConstants::attribute_id_attribute_vendor_id, tncfhh::iml::XACMLConstants::data_type_xml_string, "0x000000");
			DOMElement* attributeProductInformationType = tncfhh::iml::XACMLHelper::createAttribute(request, tncfhh::iml::XACMLConstants::attribute_id_attribute_type, tncfhh::iml::XACMLConstants::data_type_xml_string, "2");
			DOMElement* attributeProductInformationVendorID = tncfhh::iml::XACMLHelper::createAttribute(request, tncfhh::iml::XACMLConstants::attribute_id_product_information_vendor_id, tncfhh::iml::XACMLConstants::data_type_xml_string, "0x032715");
			DOMElement* attributeProductInformationProductID = tncfhh::iml::XACMLHelper::createAttribute(request, tncfhh::iml::XACMLConstants::attribute_id_product_information_product_id, tncfhh::iml::XACMLConstants::data_type_xml_string, "0x045612");
			DOMElement* attributeProductInformationProdcutName = tncfhh::iml::XACMLHelper::createAttribute(request, tncfhh::iml::XACMLConstants::attribute_id_product_information_product_name, tncfhh::iml::XACMLConstants::data_type_xml_string, "ClamAV");
			tncfhh::iml::XACMLHelper::addAttributeToSubject(subjectProductInformation, attributeProductInformationAttributeVendorID);
			tncfhh::iml::XACMLHelper::addAttributeToSubject(subjectProductInformation, attributeProductInformationType);
			tncfhh::iml::XACMLHelper::addAttributeToSubject(subjectProductInformation, attributeProductInformationVendorID);
			tncfhh::iml::XACMLHelper::addAttributeToSubject(subjectProductInformation, attributeProductInformationProductID);
			tncfhh::iml::XACMLHelper::addAttributeToSubject(subjectProductInformation, attributeProductInformationProdcutName);
			tncfhh::iml::XACMLHelper::addSubjectToRequest(request, subjectProductInformation);

			DOMElement* subjectNumericVersion = tncfhh::iml::XACMLHelper::createSubject(request, tncfhh::iml::XACMLConstants::subject_category_tnc_ifm_attribute_numeric_version);
			DOMElement* attributeNumericVersionVendorID = tncfhh::iml::XACMLHelper::createAttribute(request, tncfhh::iml::XACMLConstants::attribute_id_attribute_vendor_id, tncfhh::iml::XACMLConstants::data_type_xml_string, "0x000000");
			DOMElement* attributeNumericVersionType = tncfhh::iml::XACMLHelper::createAttribute(request, tncfhh::iml::XACMLConstants::attribute_id_attribute_type, tncfhh::iml::XACMLConstants::data_type_xml_string, "3");
			tncfhh::iml::XACMLHelper::addAttributeToSubject(subjectNumericVersion, attributeNumericVersionVendorID);
			tncfhh::iml::XACMLHelper::addAttributeToSubject(subjectNumericVersion, attributeNumericVersionType);

			for (std::vector<Measurement>::iterator iter = measurements->begin(); iter != measurements->end(); ++iter) {
				if (iter->key == OperationalStatus) {
					std::map<std::string, std::string> operationalStatusses;
					operationalStatusses["unknown"] = "0";
					operationalStatusses["not installed"] = "1";
					operationalStatusses["installed"] = "2";
					operationalStatusses["operational"] = "3";

					DOMElement* subjectOperationalStatus = tncfhh::iml::XACMLHelper::createSubject(request, tncfhh::iml::XACMLConstants::subject_category_tnc_ifm_attribute_operational_status);
					DOMElement* attributeOperationalStatusVendorID = tncfhh::iml::XACMLHelper::createAttribute(request, tncfhh::iml::XACMLConstants::attribute_id_attribute_vendor_id, tncfhh::iml::XACMLConstants::data_type_xml_string, "0x000000");
					DOMElement* attributeOperationalStatusType = tncfhh::iml::XACMLHelper::createAttribute(request, tncfhh::iml::XACMLConstants::attribute_id_attribute_type, tncfhh::iml::XACMLConstants::data_type_xml_string, "5");
					DOMElement* attributeOperationalStatusStatus = tncfhh::iml::XACMLHelper::createAttribute(request, tncfhh::iml::XACMLConstants::attribute_id_operational_status_status, tncfhh::iml::XACMLConstants::data_type_xml_string, operationalStatusses[iter->value]);
					tncfhh::iml::XACMLHelper::addAttributeToSubject(subjectOperationalStatus, attributeOperationalStatusVendorID);
					tncfhh::iml::XACMLHelper::addAttributeToSubject(subjectOperationalStatus, attributeOperationalStatusType);
					tncfhh::iml::XACMLHelper::addAttributeToSubject(subjectOperationalStatus, attributeOperationalStatusStatus);
					tncfhh::iml::XACMLHelper::addSubjectToRequest(request, subjectOperationalStatus);
				} else if (iter->key == Version) {
					/**
					 * First, replaces all . with whitespace, then uses integer-stringstream to write every part into three integer variables
					 */
					std::string value = iter->value;
					unsigned int position = value.find("."); // find first space
					while (position != std::string::npos)
					{
						value.replace(position, 1, " ");
						position = value.find(".", position + 1 );
					}

					std::string major;
					std::string minor;
					std::string service_pack;
					std::stringstream sst;
					sst.str(value);
					sst >> major;
					sst >> minor;
					sst >> service_pack;

					// when version number only consists of two parts (0.97) instead of three (0.96.5), fill the last part with 0
					if (service_pack == "") {
						service_pack = "0";
					}

					DOMElement* attributeNumericVersionProductVersionMajor = tncfhh::iml::XACMLHelper::createAttribute(request, tncfhh::iml::XACMLConstants::attribute_id_attribute_numeric_version_major_version, tncfhh::iml::XACMLConstants::data_type_xml_integer, major);
					DOMElement* attributeNumericVersionProductVersionMinor = tncfhh::iml::XACMLHelper::createAttribute(request, tncfhh::iml::XACMLConstants::attribute_id_attribute_numeric_version_minor_version, tncfhh::iml::XACMLConstants::data_type_xml_integer, minor);
					DOMElement* attributeNumericVersionProductVersionServicePackMajor = tncfhh::iml::XACMLHelper::createAttribute(request, tncfhh::iml::XACMLConstants::attribute_id_attribute_numeric_version_service_pack_major, tncfhh::iml::XACMLConstants::data_type_xml_integer, service_pack);
					tncfhh::iml::XACMLHelper::addAttributeToSubject(subjectNumericVersion, attributeNumericVersionProductVersionMajor);
					tncfhh::iml::XACMLHelper::addAttributeToSubject(subjectNumericVersion, attributeNumericVersionProductVersionMinor);
					tncfhh::iml::XACMLHelper::addAttributeToSubject(subjectNumericVersion, attributeNumericVersionProductVersionServicePackMajor);
				} else if (iter->key == Main_Version) {
					DOMElement* attributeNumericVersionProductVersionServicePackMinor = tncfhh::iml::XACMLHelper::createAttribute(request, tncfhh::iml::XACMLConstants::attribute_id_attribute_numeric_version_service_pack_minor, tncfhh::iml::XACMLConstants::data_type_xml_integer, iter->value);
					tncfhh::iml::XACMLHelper::addAttributeToSubject(subjectNumericVersion, attributeNumericVersionProductVersionServicePackMinor);
				} else if (iter->key == Daily_Version) {
					DOMElement* attributeNumericVersionBuildNumber = tncfhh::iml::XACMLHelper::createAttribute(request, tncfhh::iml::XACMLConstants::attribute_id_attribute_numeric_version_build_number, tncfhh::iml::XACMLConstants::data_type_xml_integer, iter->value);
					tncfhh::iml::XACMLHelper::addAttributeToSubject(subjectNumericVersion, attributeNumericVersionBuildNumber);
				}
			}
			tncfhh::iml::XACMLHelper::addSubjectToRequest(request, subjectNumericVersion);
			tncfhh::iml::XACMLHelper::addResourceActionEnvironmentToRequest(request);

			/**
			 * Evaluate the request by an XACML-PDP.
			 */
			TNC_IMV_Action_Recommendation recommendation = tncfhh::iml::XACMLHelper::getXACMLRecommendation(request);

			if (recommendation != TNC_IMV_ACTION_RECOMMENDATION_NO_RECOMMENDATION) {
				this->actionRecommendation = recommendation;
				if (recommendation == TNC_IMV_ACTION_RECOMMENDATION_ALLOW) {
					this->evaluationResult = TNC_IMV_EVALUATION_RESULT_COMPLIANT;
					LOG4CXX_DEBUG(logger, "XACML evaluation resulted in 'ALLOW'");
				} else if (recommendation == TNC_IMV_ACTION_RECOMMENDATION_ISOLATE) {
					this->evaluationResult = TNC_IMV_EVALUATION_RESULT_NONCOMPLIANT_MAJOR;
					LOG4CXX_DEBUG(logger, "XACML evaluation resulted in 'ISOLATE'");
				} else if (recommendation == TNC_IMV_ACTION_RECOMMENDATION_NO_ACCESS) {
					this->evaluationResult = TNC_IMV_EVALUATION_RESULT_NONCOMPLIANT_MAJOR;
					LOG4CXX_DEBUG(logger, "XACML evaluation resulted in 'NO ACCESS'");
				}
				this->validationFinished = true;
				this->tncs.provideRecommendation(this->actionRecommendation, this->evaluationResult);

				return true;
			} else {
				LOG4CXX_DEBUG(logger, "No evaluation by XACML-PDP.");
				return false;
			}
		} else {
			LOG4CXX_DEBUG(logger, "Username could not be read");
			return false;
		}
	} catch (tncfhh::iml::ResultException &e) {
		LOG4CXX_DEBUG(logger, "Username could not be read");
		return false;
	}
}
