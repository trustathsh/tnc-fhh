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
  */

/** @file
 * TNC@FHH specific addons to the TCG specifications
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>10.11.2008 - created the header with remaining definitions from the old fhhaddon.h (bhl)</li>
 * </ul>
 *
 * @date 10.11.2008
 * @author: Bastian Hellmann (bhl)
 */

#ifndef TNC_IMUNIT_H_
#define TNC_IMUNIT_H_

/**
 * IMUnit ID is the generalization of IMC/IMV ID.
 */
typedef unsigned long TNC_IMUnitID;

/**
 * Function pointer to bind function.
 */
typedef TNC_Result (*TNC_BindFunctionPointer)(
               TNC_IMUnitID id,
              char *functionName,
               void **pOutfunctionPointer
);

#endif /* TNC_IMUNIT_H_ */
