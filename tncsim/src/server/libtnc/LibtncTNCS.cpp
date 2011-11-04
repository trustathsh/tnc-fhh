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
 
#include "LibtncTNCS.h"

#include <iostream>

namespace tncsim {

namespace server {

server::AbstractTNCS * getServer(std::string &tncConfig)
{
	return new libtnc::LibtncTNCS(tncConfig);
}

namespace libtnc {

#define CHECK(x) if ((x)!=TNC_RESULT_SUCCESS){std::cerr<<"error in "<<__FILE__<<":"<<__LINE__<<std::endl;throw std::runtime_error("Error in libtnc.");}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
LibtncTNCS::LibtncTNCS(std::string &tncConfig)
	:AbstractTNCS(tncConfig), myImvConn(NULL)
{
	CHECK(libtnc_tncs_Initialize(this->tncConfig.c_str()));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
LibtncTNCS::~LibtncTNCS()
{
	if (myImvConn != NULL)
		libtnc_tncs_DeleteConnection(myImvConn);
	CHECK(libtnc_tncs_Terminate());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void LibtncTNCS::createConnection(TNC_ConnectionID)
{
	this->myImvConn = libtnc_tncs_CreateConnection(NULL);
	CHECK(libtnc_tncs_BeginSession(myImvConn));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
static TNCCSData data(0);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNCCSData LibtncTNCS::receiveTNCCSData(const TNCCSData & data) throw(Finished)
{
	TNC_IMV_Action_Recommendation recommendation = 0;
	TNC_IMV_Evaluation_Result evaluation = 0;

	CHECK(libtnc_tncs_ReceiveBatch(this->myImvConn, (const char *)data.getData(), data.getLength()));
	if (libtnc_tncs_HaveRecommendation(myImvConn, &recommendation, &evaluation) == TNC_RESULT_SUCCESS)
		throw Finished(recommendation, tncsim::server::libtnc::data);
	return tncsim::server::libtnc::data;
}

}

}

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result TNC_TNCS_SendBatch(
    /*in*/ libtnc_tncs_connection* conn,
    /*in*/ const char* messageBuffer,
    /*in*/ size_t messageLength)
{
	tncsim::server::libtnc::data.setData((const unsigned char *)messageBuffer, messageLength);

	return TNC_RESULT_SUCCESS;
}
