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
#include "AbstractTNCCSProtocol.h"
#include <log4cxx/logger.h>

namespace tncfhh {

namespace iel {

/* for logging */
static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("TNCS.AbstractTNCCSProtocol"));

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
AbstractTNCCSProtocol::AbstractTNCCSProtocol(const std::map<TNC_IMVID, IMV*> &imvsMap)
:imvs(imvsMap)
{
	//Do nothing? Really? Yes! NO! YES!
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
AbstractTNCCSProtocol::~AbstractTNCCSProtocol() {
	//Do nothing, too.
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::string AbstractTNCCSProtocol::getProtocolName() const
{
	return this->protocolName;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//void AbstractTNCCSProtocol::setIMVs(const std::map<TNC_IMVID, IMV*> &imvsMap)
//{
//
//    std::map<TNC_IMVID, IMV*>::const_iterator iter = imvsMap.begin();
//    while (iter!=imvsMap.end())
//    {
//    	if (!(iter->second))
//        {
//    		// ignore NULL
//        	LOG4CXX_WARN(logger, "Uhoh strange things happening...imv instance == NULL");
//        	iter++;
//        	continue;
//        }
//
//    	LOG4CXX_DEBUG(logger, "Adding IMV " << (*iter).second->getIMVProperties().getName() << " to TNCCSProtocol " << getProtocolName());
//
//        this->imvs.push_back(iter->second);
//
//       	iter++;
//     }
//}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void AbstractTNCCSProtocol::notifyConnectionChangeHandshake()
{
	// TODO AbstractTNCCSProtocol::notifyConnectionChangeHandshake()
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *        * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *        *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
TNCCSData AbstractTNCCSProtocol::processTNCCSData(TNCCSData incomingTNCCSData) throw (ConnectionStateException, TNCCSProtocolNotSupportedException)
{
	throw TNCCSProtocolNotSupportedException("AbstractTNCCSProtocol does not support anything.");
}

} // namespace iel

} // tncfhh
