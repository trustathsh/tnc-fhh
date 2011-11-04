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
 
#ifndef LIBTNCTNCC_H_
#define LIBTNCTNCC_H_

#include "../AbstractTNCC.h"

#include <libtncimc.h>
#include <libtnctncc.h>


namespace tncsim {

namespace client {

namespace libtnc {

/**
 * Implementation of abstract interface class for TNCCs.
 * This is basically a wrapper class between tncsim and libtnc's TNCC functions.
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>16.7.2009 - first implementation (mbs)</li>
 *   <li>16.03.2010 - integrated tncsim from branches to tncfhh (ib)</li>
 * </ul>
 *
 * @date 16.03.2010
 * @author Mike Steinmetz (mbs)
 * @author Ingo Bente (ib)
 */
class LibtncTNCC: public tncsim::client::AbstractTNCC {
public:
	LibtncTNCC(std::string &tncConfig);
	virtual ~LibtncTNCC();

	virtual void createConnection(TNC_ConnectionID);
	virtual TNCCSData beginHandshake();
	virtual TNCCSData receiveTNCCSData(const TNCCSData & data);
private:
	libtnc_tncc_connection* myImcConn;
};

}

}

}

#endif /* LIBTNCTNCC_H_ */
