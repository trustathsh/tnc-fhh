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
 
/*
 * LibTNCClient.h
 *
 *  Created on: Jul 31, 2010
 *      Author: awelzel
 */

#ifndef LIBTNCCLIENT_H_
#define LIBTNCCLIENT_H_

/* libtnc header */
#include <libtnctncc.h>

#include "tncvpn/ifttls/Envelope.h"

namespace tnc {

class LibTNCClient {

public:
	LibTNCClient();
	~LibTNCClient();

	void beginSession();

	/* you are responsible for freeing the returned pointer */
	ifttls::Envelope *getMessage();

	/* you are responsible for freeing the pointer given */
	void putData(ifttls::Envelope *env);

	bool wantRead() const { return wantRead_; };
	bool wantWrite() const { return wantWrite_; };

	bool isFinished() const { return finished_; };


private:
	bool wantRead_, wantWrite_, finished_;
	libtnc_tncc_connection *connection;
};

} // namespace tnc

extern "C" {

TNC_Result TNC_TNCC_SendBatch( libtnc_tncc_connection *conn, const char *buffer,
			       size_t len);

TNC_Result TNC_TNCS_SendBatch( libtnc_tncc_connection *conn, const char *buffer,
			       size_t len);
}




#endif /* LIBTNCCLIENT_H_ */
