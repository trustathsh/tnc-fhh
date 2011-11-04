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
 
#include "PcrSelection.h"

/* log4cxx includes */
#include <log4cxx/logger.h>
using namespace log4cxx;

static LoggerPtr
logger(Logger::getLogger("PcrSelection"));


PcrSelection::PcrSelection(TNC_BufferReference bitmask, UINT16 length)
	: selectedPcrs()
	  , maxPcr(0)
{
	init(bitmask, length);
}

PcrSelection::~PcrSelection() {
	// TODO Auto-generated destructor stub
}


/*
 * Main_TCG_Architecture_v1_1b 4.25.3
 *
 * The first two bytes of the message represent the length
 * of the bitmask that follows. The bitmask represents the
 * requested PCRs to be quoted.
 *
 * The bitmask is big endian and "should look like:"
 *
 *        BYTE 1             BYTE 2                   ...
 * Bit:   1 1 1 1 0 0 0 0    1  1  1  1  0  0  0 0    ...
 * Pcr:   7 6 5 4 3 2 1 0    15 14 13 12 11 10 9 8    ...
 *
 *
 */
void PcrSelection::init(TNC_BufferReference br, UINT16 length) {
	if (length < 2) {
		LOG4CXX_FATAL(logger, "PcrSelection length < 2");
		return;
	}
	LOG4CXX_TRACE(logger, "Bitmask length=" << length);

	// bytewise through the bitmask
	for(int i = 0; i < length; i++) {
		// bitwise through the byte
		for(int j = 0; j < 8; j++) {
			if (br[i] & (1 << j)) {
				selectedPcrs.push_back(i * 8 + j);
				maxPcr = i * 8 + j;
			}
		}
	}

	LOG4CXX_INFO(logger, "Requested PCRS:");

	for (std::list<long>::iterator it = selectedPcrs.begin(); it != selectedPcrs.end(); it++) {
		LOG4CXX_INFO(logger, "PCR" << *it);
	}
}

std::list<long>& PcrSelection::getPcrList(void)
{
	return selectedPcrs;
}

long PcrSelection::getHighestPcr(void)
{
	return maxPcr;
}

UINT16 PcrSelection::getPcrCount(void)
{
	return selectedPcrs.size();
}




