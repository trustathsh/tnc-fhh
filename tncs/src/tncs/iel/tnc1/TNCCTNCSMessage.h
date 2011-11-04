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
#ifndef TNCS_TNCCTNCSMESSAGE_H_
#define TNCS_TNCCTNCSMESSAGE_H_

#include <tcg/tnc/tncifimv.h>

namespace tncfhh {

namespace iel {

namespace tnc1 {


/**
 * This Class represent an TNCC-TNCS-Message. This message is specified in
 * TNC IF-TNCCS. It exist two types of TNCC-TNCS-Message, binary base and XML
 * base messages.
 * 
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>25.02.2008 - create class (mbs)</li>
 *   <li>29.02.2008 - (ib)</li>
 *   <li>05.03.2008 - split class (mbs)</li>
 *   <li>20.07.2009 - redesign for 0.6.0 (mbs)</li>
 * </ul>
 *
 * @class TNCCTNCSMessage
 * @brief This Class represent an TNCC-TNCS-Message.
 * @date 25.02.2008
 * @author Mike Steinmetz (mbs)
 * @author Ingo Bente (ib)
 * @see TNCCTNCSMessageBase64
 * @see TNCCTNCSMessageXML
 */
class TNCCTNCSMessage
{
public:
	virtual TNC_MessageType getType();
	virtual ~TNCCTNCSMessage();
	
protected:
	TNCCTNCSMessage(TNC_MessageType type);

private:
	TNC_MessageType type;
	
};

}  // namespace tnc1

}  // namespace iel

}  // namespace tncfhh

#endif /*TNCS_TNCCTNCSMESSAGE_H_*/
