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
 
#ifndef ACKNOWLEDGEMENTEXCEPTION_H_
#define ACKNOWLEDGEMENTEXCEPTION_H_

#include <exception>

namespace tncfhh {

namespace nal {

/**
 * TODO_Comment
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>22.07.2009 - create class(mbs)</li>
 * </ul>
 *
 * @class AcknowledgementException
 * @brief TODO_Brief
 * @date 22.07.2009
 * @author Mike Steinmetz (mbs)
 */
class AcknowledgementException : public std::exception {
public:
	AcknowledgementException() throw ();
	virtual ~AcknowledgementException() throw ();
};

}

}

#endif /* ACKNOWLEDGEMENTEXCEPTION_H_ */
