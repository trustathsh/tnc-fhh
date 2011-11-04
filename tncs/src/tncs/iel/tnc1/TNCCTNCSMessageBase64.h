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
#ifndef TNCS_TNCCTNCSMESSAGEBASE64_H_
#define TNCS_TNCCTNCSMESSAGEBASE64_H_

#include "TNCCTNCSMessage.h"

namespace tncfhh {

namespace iel {

namespace tnc1 {

/**
 * This Class represent an TNCC-TNCS-Message with binary data. This message
 * is specified in TNC IF-TNCCS.
 * 
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>05.03.2008 - create class (mbs)</li>
 *   <li>20.07.2009 - redesign for 0.6.0 (mbs)</li>
 * </ul>
 *
 * @class TNCCTNCSMessageBase64
 * @brief Represent an TNCC-TNCS-Message with binary data.
 * @date 05.03.2008
 * @author Mike Steinmetz (mbs)
 * @see TNCCTNCSMessage
 * @see TNCCTNCSMessageXML
 */
class TNCCTNCSMessageBase64 : public TNCCTNCSMessage
{
public:
	TNCCTNCSMessageBase64(TNC_MessageType type, TNC_BufferReference data, TNC_UInt32 dataLength);
	virtual ~TNCCTNCSMessageBase64();
	virtual TNC_BufferReference getData();
	virtual TNC_UInt32 getDataLength();
private:
	TNC_BufferReference data;
	TNC_UInt32 dataLength;
};

}  // namespace tnc1

}  // namespace iel

}  // namespace tncfhh

#endif /*TNCS_TNCCTNCSMESSAGEBASE64_H_*/
