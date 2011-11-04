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
 
#include "TncfhhTNCS.h"

namespace tncsim {

namespace server {

server::AbstractTNCS * getServer(std::string &tncConfig)
{
	return new tncfhh::TncfhhTNCS(tncConfig);
}

namespace tncfhh {

TncfhhTNCS::TncfhhTNCS(std::string &tncConfig)
	:AbstractTNCS(tncConfig)
{
	::tncfhh::iel::coordinator.initialize(this->tncConfig);
}

TncfhhTNCS::~TncfhhTNCS()
{
	::tncfhh::iel::coordinator.terminate();
}

void TncfhhTNCS::createConnection(TNC_ConnectionID)
{
	this->cid = ::tncfhh::iel::coordinator.getConnection("");
}

TNCCSData TncfhhTNCS::receiveTNCCSData(const TNCCSData & data) throw(Finished)
{
	try {
		::tncfhh::iel::TNCCSData ielInData(data.getData(), data.getLength());
		::tncfhh::iel::TNCCSData ielOutData = ::tncfhh::iel::coordinator.processTNCCSData(cid, ielInData);
		return TNCCSData(ielOutData.getData(), ielOutData.getLength());
	} catch (::tncfhh::iel::ConnectionStateExceptionWithTNCCSData & e) {
		throw Finished(e.getConnectionState(), TNCCSData(e.getLastTnccsData().getData(), e.getLastTnccsData().getLength()));
	} catch (::tncfhh::iel::ConnectionStateException & e) {
		throw Finished(e.getConnectionState(), TNCCSData(0));
	}
}

}

}

}
