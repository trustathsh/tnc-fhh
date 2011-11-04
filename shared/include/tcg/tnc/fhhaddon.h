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
 
/**
 * TNC@FHH specific addons to the TCG specifications
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>15.08.2006 - create (MS)</li>
 *   <li>15.02.2008 - comments/change (ib)</li>
 *   <li>11.04.2008 - add connectionCreate (mbs)</li>
 *   <li>22.07.2009 - redesign for TNC@FHH 0.6.0 (mbs)</li>
 * </ul>
 *
 * @date 15.08.2006
 * @author Martin Schmiedel (MS)
 * @author: Mike Steinmetz (mbs)
 * @author: Ingo Bente (ib)
 */
#ifndef FHHADDON_H_
#define FHHADDON_H_

#define EAP_TNC_HEADER_FLAGS_SIZE        1
#define EAP_TNC_HEADER_DATA_LENGTH_SIZE  4

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
#ifdef TNC_IMV_EXPORTS
#define TNC_IMV_API __declspec(dllexport)
#else
#define TNC_IMV_API __declspec(dllimport)
#endif
#else
#define TNC_IMV_API
#endif

/**
 * For convenience.
 */
typedef unsigned char TNC_Buffer;

/**
 * For convenience.
 */
typedef const unsigned char * TNC_BufferConstReference;

/**
 * For using user-specific policies
 */
#define TNC_ATTRIBUTEID_USERNAME ((TNC_AttributeID) 0x10000001)

#ifdef __cplusplus
}
#endif

#endif /*FHHADDON_H_*/
