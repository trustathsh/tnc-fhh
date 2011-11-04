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
 * For std::string + X.
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>12.07.2009 - create & implement functions (mbs)</li>
 *   <li>20.07.2009 - added namespace (ib)</li>
 * </ul>
 *
 * @file string_helper.h
 * @date 12.07.2009
 * @author Mike Steinmetz (mbs)
 */

#ifndef STRING_HELPER_H_
#define STRING_HELPER_H_

#include <sstream>

namespace tncfhh
{

/* for std::string + std::string */
inline std::string operator+(std::string const& l, std::string const& r)
{
	std::stringstream s;
	s<<l<<r;
	return s.str();
}

/* for std::string + unsigned int */
inline std::string operator+(std::string const& l, unsigned int const& r)
{
	std::stringstream s;
	s<<l<<r;
	return s.str();
}

/* for std::string + "" */
inline std::string operator+(std::string const& l, char const r[])
{
	std::stringstream s;
	s<<l<<r;
	return s.str();
}

/* for std::string + pointer */
inline std::string operator+(std::string const& l, const void * r)
{
	std::stringstream s;
	s<<l<<r;
	return s.str();
}

} // namespace tncfhh

#endif /*STRING_HELPER_H_*/
