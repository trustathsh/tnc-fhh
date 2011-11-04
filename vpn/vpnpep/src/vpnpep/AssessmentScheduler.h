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
 
#ifndef ASSESSMENT_SCHEDULER_H
#define ASSESSMENT_SCHEDULER_H

#include <iostream>
#include <string>

#include "EndpointDB.h"
#include "EnforcementManager.h"

namespace tnc {


/**
 * The assessment scheduler performs checks every 10 seconds
 * wether a new assessment for an endpoint is required or not.
 */
class AssessmentScheduler {
private:

	/**
	 * The endpoint db stores all endpoints and the corresponding
	 * timstamp of their last assessment
	 */
	tnc::EndpointDB *db;

protected:


public:
	/**
	 * Constructs a new assessment_scheduler and initializes
	 * the endpoint_db.
	 */
	AssessmentScheduler(EndpointDB *db)
		: db(db) { }

	virtual ~AssessmentScheduler() {

	}


	/**
	 * Main method of the assessment_scheduler. The operator() is
	 * used by the boost thread library as "main" method of the
	 * thread.
	 */
	void operator()() {
		while (true) {
			std::cout << "[Scheduler] Looking for reassessments" << std::endl;

			// Getting the current time
			time_t current_time = time(NULL);
			struct tm current_ts;
			localtime_r(&current_time, &current_ts);


			// For each endpoint in the database
			for(EP_HashMap::iterator it = db->getEndpoints()->begin(); it != db->getEndpoints()->end(); ++it) {
				Endpoint* ep = it->second;
				if (ep == 0) continue;

				struct tm ts;
				localtime_r(ep->getLastAssessment(), &ts);

				// Calculating the difference
				int min = current_ts.tm_min - ts.tm_min;

				// Reassessment are expected within an interval of 5 minutes
				if (min > 5) {
					std::string ip = ep->getAddress()->to_string();
					std::cout << "[Scheduler] WARN: Reassessment for " << ip << " is missing!" << std::endl;
					std::cout << "[Scheduler] WARN: Last assessment was on " << ts.tm_hour << ":" << ts.tm_min << ":" << ts.tm_sec << std::endl;

					// Revoking the access to the network
					enforceBinaryIso((char*)ip.c_str(), 'N');

					// Deleting the endpoint from the db
					delete ep;
					it->second = 0;


				}

			}

			sleep(10);
		}
	}

};

}

#endif
