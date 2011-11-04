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
#ifndef TNCS_IMCIMVMESSAGE_H_
#define TNCS_IMCIMVMESSAGE_H_

#include <tcg/tnc/tncifimv.h>
#include <tcg/tnc/fhhaddon.h>

namespace tncfhh
{

namespace iel
{

namespace tnc1
{

/**
 * TODO_Comment
 * 
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>25.02.2008 - create class (mbs)</li>
 *   <li>28.02.2008 - add content (mbs)</li>
 *   <li>16.07.2009 - redesign 0.6.0 (ib)</li>
 * </ul>
 *
 * @class IMCIMVMessage
 * @brief TODO_Brief
 * @date 25.02.2008
 * @author Mike Steinmetz (mbs)
 *         Ingo Bente (ib)
 */
class IMCIMVMessage
{
public:
	/**
	 * Create an IMC-IMV-Message.
	 *
	 * @param data this binary data is delete by ~IMCIMVMessage() (delete[])
	 * @param dataLength the length of binary data
	 * @param type the MessageType
	 */
	IMCIMVMessage(TNC_BufferReference data, TNC_UInt32 dataLength, TNC_MessageType type);
	virtual ~IMCIMVMessage();
	
	virtual TNC_BufferReference getData();
	virtual TNC_UInt32 getDataLength();
	virtual TNC_MessageType getType();
private:
	TNC_BufferReference data;
	TNC_UInt32 dataLength;
	TNC_MessageType type;
};

}  // namespace tnc1

}  // namespace iel

}  // namespace tncfhh

#endif /*TNCS_IMCIMVMESSAGE_H_*/
