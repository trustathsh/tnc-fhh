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
 
#include "HostScannerIMV.h"

#include <imunit/exception/ResultException.h>

#include <errno.h>
#include <stdlib.h>
#include <fstream>
#include <string.h>

// include log4cxx header files.
#include "log4cxx/logger.h"
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/helpers/exception.h"
using namespace log4cxx;
using namespace log4cxx::helpers;

static LoggerPtr logger(Logger::getLogger("IMUnit.AbstractIMUnit.AbstractIMV.HostScannerIMV"));

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
HostScannerIMV::HostScannerIMV(TNC_ConnectionID conID, HostScannerIMVLibrary *pHostScannerIMVLibrary)
    : tncfhh::iml::AbstractIMV(conID, pHostScannerIMVLibrary), count(0)
{
	//Western
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
HostScannerIMV::~HostScannerIMV()
{
	//Egg
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result HostScannerIMV::notifyConnectionChange()
{
	if (this->getConnectionState() == TNC_CONNECTION_STATE_HANDSHAKE) {

		this->policys.clear();
	    this->count = 0;

	    std::ifstream policy(IMV_CONFIG, std::ios::in);
	    if (!policy)
	    {
	    	LOG4CXX_WARN(logger, "policy file not found: " << IMV_CONFIG);
	    	throw ::tncfhh::iml::ResultException("policy file not found", TNC_RESULT_OTHER);
	    }

	    std::vector<std::pair<std::string,std::string> > properties = readAllProperties(policy);

	    policy.close();

		for (std::vector<std::pair<std::string,std::string> >::iterator iter = properties.begin();
		    iter != properties.end();
		    ++iter) {

			/* read Protocol */
			Protocol protocol;
			if (!iter->first.compare(0, 3, "TCP"))
				protocol = TCP;
			else if (!iter->first.compare(0, 3, "UDP"))
				protocol = UDP;
			else
				throw ::tncfhh::iml::ResultException("Protocol is unknown (not TCP or UDP)", TNC_RESULT_OTHER);

			/* read Port */
			size_t tmpPos = iter->first.find_first_of("1234567890", 3);
			if (tmpPos==std::string::npos)
				throw ::tncfhh::iml::ResultException("No Portnumber found", TNC_RESULT_OTHER);
			char * end = NULL;
			unsigned long int port = strtoul(iter->first.substr(tmpPos, iter->first.size()).c_str(), &end, 10);
			if (errno==ERANGE || errno==EINVAL /*|| *end */) // FIXME valgrind memory invalid read
				throw ::tncfhh::iml::ResultException("HostScannerIMV(): Error by string to unsigned int.", TNC_RESULT_OTHER);

			/* read Status */
			Status status;
			if (!iter->second.compare("open"))
				status = open;
			else if (!iter->second.compare("close"))
				status = close;
			else if (!iter->second.compare("whatever"))
				status = whatever;
			else
				throw ::tncfhh::iml::ResultException("Status is unknown (not open, close or whatever)", TNC_RESULT_OTHER);

			this->policys.push_back(Policy(protocol, (unsigned short) port, status));
		}
	}

	return TNC_RESULT_SUCCESS;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result HostScannerIMV::receiveMessage(TNC_BufferReference message, TNC_UInt32 messageLength, TNC_MessageType messageType)
{
    LOG4CXX_DEBUG(logger, "Receive Message: Length = " << messageLength  << " Bytes.");
    TNC_Result ret = TNC_RESULT_SUCCESS;

    if (this->count == 0)
    {
    	std::stringstream send;

    	for (std::vector<Policy>::iterator iter = this->policys.begin();
    	    iter != this->policys.end();
    	    ++iter) {
    		send << (iter->protocol==TCP?"TCP":"UDP") << " " << std::dec << iter->port << std::endl;
    	}

    	std::string sendString = send.str();

        LOG4CXX_DEBUG(logger, "Send Message: Length = " << sendString.size() << " Bytes");
    	this->tncs.sendMessage((unsigned char*)sendString.c_str(), sendString.size(), TNC_MESSAGETYPE_FHH_HOSTSCANNER);
    } else if (this->count == 1) {
    	/* second package */
    	std::stringstream ss;

    	ss.write((const char *)message, messageLength);

	    std::vector<std::pair<std::string,std::string> > properties = readAllProperties(ss);

    	std::vector<Policy> policys;

		for (std::vector<std::pair<std::string,std::string> >::iterator iter = properties.begin();
		    iter != properties.end();
		    ++iter) {

			/* read Protocol */
			Protocol protocol;
			if (!iter->first.compare(0, 3, "TCP"))
				protocol = TCP;
			else if (!iter->first.compare(0, 3, "UDP"))
				protocol = UDP;
			else
			{
				std::stringstream errorss;
				errorss << "Protocol is unknown (not TCP or UDP): " << iter->first;
				return this->clientError(errorss.str());
			}

			/* read Port */
			size_t tmpPos = iter->first.find_first_of("1234567890", 3);
			if (tmpPos==std::string::npos)
			{
				std::stringstream errorss;
				errorss << "No port number found: " << iter->first;
				return this->clientError(errorss.str());
			}

			char * end = NULL;
			unsigned long int port = strtoul(iter->first.substr(tmpPos, iter->first.size()).c_str(), &end, 10);
			if (errno==ERANGE || errno==EINVAL /*|| *end*/) // FIXME valgrind memory invalid read
			{
				std::stringstream errorss;
				errorss << "Error by string to unsigned int: " << iter->first;
				return this->clientError(errorss.str());
			}

			/* read Status */
			Status status;
			if (!iter->second.compare("open"))
				status = open;
			else if (!iter->second.compare("close"))
				status = close;
			else if (!iter->second.compare("whatever"))
				status = whatever;
			else
			{
				std::stringstream errorss;
				errorss << "Status is unknown (not open, close or whatever): " << iter->second;
				return this->clientError(errorss.str());
			}

			policys.push_back(Policy(protocol, (unsigned short) port, status));
		}

		for (std::vector<Policy>::iterator iter = this->policys.begin();
		    iter != this->policys.end();
		    ++iter) {
			for (std::vector<Policy>::iterator iterC = policys.begin();
			    iterC != policys.end();
			    ++iterC) {
				if (iter->protocol ==iterC->protocol &&
					iter->port == iterC->port &&
					iter->status != whatever) {
					if (iter->status != iterC->status) {
						this->actionRecommendation = TNC_IMV_ACTION_RECOMMENDATION_NO_ACCESS;
						this->evaluationResult = TNC_IMV_EVALUATION_RESULT_NONCOMPLIANT_MAJOR;
						this->validationFinished = true;
						this->tncs.provideRecommendation(this->actionRecommendation, this->evaluationResult);
						return TNC_RESULT_SUCCESS;
					}
				}
			}
		}
		this->actionRecommendation = TNC_IMV_ACTION_RECOMMENDATION_ALLOW;
		this->evaluationResult = TNC_IMV_EVALUATION_RESULT_COMPLIANT;
		this->validationFinished = true;
		this->tncs.provideRecommendation(this->actionRecommendation, this->evaluationResult);
		return TNC_RESULT_SUCCESS;
    }

    return ret;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result HostScannerIMV::batchEnding()
{
	this->count++;

	return TNC_RESULT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result HostScannerIMV::clientError(std::string s)
{
	this->actionRecommendation = TNC_IMV_ACTION_RECOMMENDATION_NO_RECOMMENDATION;
	this->evaluationResult = TNC_IMV_EVALUATION_RESULT_ERROR;
	this->validationFinished = true;
	LOG4CXX_WARN(logger, s)
	return TNC_RESULT_OTHER;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
HostScannerIMV::Policy::Policy(Protocol pr, unsigned short po, Status s)
:protocol(pr), port(po), status(s)
{/*wee*/}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool HostScannerIMV::readLine(std::istream &in, char *buf, const int size)
{
    bool ret = in.getline(buf, size);
    if (ret && buf[strlen(buf) - 1] == '\r') {
        buf[strlen(buf) - 1] = '\0';
    }
    return ret;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::vector<std::pair<std::string,std::string> > HostScannerIMV::readAllProperties(std::istream &in)
{
    std::vector<std::pair<std::string,std::string> > properties;

	in.clear();
	in.seekg(0, std::ios::beg);

	unsigned int buf_len = 1000;
	char buf[buf_len];

	while (readLine(in, buf, buf_len)) {
        std::string line = trim(std::string(buf));

		/* ignore comments */
		if (line.size()<1 || line.at(0)=='#')
			continue;

        unsigned int pos = line.find_first_of('=');

        /* ignore illegal lines */
        if (pos == std::string::npos) {
        	continue;
        }

        /* get key */
        std::string key = line.substr(0, pos);
        key = trim(key);

        /* get value */
        std::string value = line.substr(pos + 1, line.size());
        value = trim(value);

        /* debug */
        LOG4CXX_DEBUG(logger, "Read Key:\"" << key << "\" Value:\"" << value.c_str() << "\"");

        /* add key-value-pair */
        properties.push_back(std::pair<std::string, std::string>(key, value));
    }

    return properties;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::string HostScannerIMV::trim(std::string const& source, char const* delims) {
  std::string result(source);
  std::string::size_type index = result.find_last_not_of(delims);
  if(index != std::string::npos)
    result.erase(++index);

  index = result.find_first_not_of(delims);
  if(index != std::string::npos)
    result.erase(0, index);
  else
    result.erase();
  return result;
}
