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
 
#include "LibtncTNCC.h"

#include <iostream>

namespace tncsim {

namespace client {

client::AbstractTNCC * getClient(std::string &tncConfig)
{
	return new client::libtnc::LibtncTNCC(tncConfig);
}

namespace libtnc {

#define CHECK(x) if ((x)!=TNC_RESULT_SUCCESS){std::cerr<<"error in "<<__FILE__<<":"<<__LINE__<<std::endl;throw std::runtime_error("Error in libtnc.");}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
LibtncTNCC::LibtncTNCC(std::string &tncConfig)
	:AbstractTNCC(tncConfig), myImcConn(NULL)
{

	CHECK(libtnc_tncc_Initialize(this->tncConfig.c_str()));
	CHECK(libtnc_tncc_PreferredLanguage("de"));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
LibtncTNCC::~LibtncTNCC() {
	if (myImcConn != NULL)
		libtnc_tncc_DeleteConnection(myImcConn);
	CHECK(libtnc_tncc_Terminate());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void LibtncTNCC::createConnection(TNC_ConnectionID)
{
	this->myImcConn = libtnc_tncc_CreateConnection(NULL);
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
TNCCSData LibtncTNCC::beginHandshake()
{
	CHECK(libtnc_tncc_BeginSession(this->myImcConn));
	return tncsim::client::libtnc::data;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNCCSData LibtncTNCC::receiveTNCCSData(const TNCCSData & data)
{
	CHECK(libtnc_tncc_ReceiveBatch(this->myImcConn, (const char *)data.getData(), data.getLength()));
	return tncsim::client::libtnc::data;
}

}

}

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNC_Result TNC_TNCC_SendBatch(
    /*in*/ libtnc_tncc_connection* conn,
    /*in*/ const char* messageBuffer,
    /*in*/ size_t messageLength)
{
	tncsim::client::libtnc::data.setData((const unsigned char *)messageBuffer, messageLength);

	return TNC_RESULT_SUCCESS;
}
