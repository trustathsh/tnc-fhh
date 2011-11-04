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
 
#include "IMVLibrary.h"

#ifdef __WINDOWS__
#  include <windows.h>
#endif // __WINDOWS__


/**
 * @file
 * @see TNCFHH_IMVLIBRARY_INITIALIZE macro
 *
 * TODO - remove this file in next release > 0.6.0
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>09.05.2006 - create class (Daniel Wuttke)</li>
 *   <li>00.02.2008 - completely redesign (ib & mbs)</li>
 *   <li>12.02.2008 - attributeFunctions set to NULL in terminate (ib)</li>
 *   <li>08.05.2008 - change namespace (mbs)</li>
 *   <li>10.05.2008 - add some try/catch-blocks and remove some bugs (mbs)</li>
 *   <li>10.05.2008 - add getImvInstances() code (mbs)</li>
 *   <li>16.06.2009 - redesign (ib)</li>
 *   <li>30.08.2009 - insert this cpp in the macro TNCFHH_IMVLIBRARY_INITIALIZE in IMVLibrary.h (mbs)</li>
 * </ul>
 *
 * @date 09.05.2006
 * @author Daniel Wuttke
 * @author Ingo Bente (ib)
 * @author Mike Steinmetz
 *
 */
