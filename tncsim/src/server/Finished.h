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
 
#ifndef FINISHED_H_
#define FINISHED_H_

#include <exception>
#include <tcg/tnc/tncifimv.h>
#include "../TNCCSData.h"

namespace tncsim {

namespace server {

/**
 * Exception that indicates the end of a TNC handshake.
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>16.7.2009 - first implementation (mbs)</li>
 *   <li>16.03.2010 - integrated tncsim from branches to tncfhh (ib)</li>
 * </ul>
 *
 * @date 16.03.2010
 * @author Mike Steinmetz (mbs)
 * @author Ingo Bente (ib)
 */
class Finished: public std::exception {
public:
	Finished(TNC_IMV_Action_Recommendation rec, TNCCSData lastData) throw();
	virtual ~Finished() throw();
	TNC_IMV_Action_Recommendation rec;
	TNCCSData lastData;
};

}

}

#endif /* FINISHED_H_ */
