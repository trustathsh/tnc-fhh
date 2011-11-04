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
 
#ifndef ENFORCEMENT_MANAGER_H
#define ENFORCEMENT_MANAGER_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

#include <tncutil/Configuration.h>

void enforceBinaryIso(char* ip, char recommendation) {
	// Create a new child process
	if (fork() == 0) {
		/** Script for the "allow" case */
		const char* allowScript = tncfhh::Configuration::instance()->get("ALLOW_SCRIPT").c_str();

		/** Script for the "deny" case */
		const char* denyScript = tncfhh::Configuration::instance()->get("DENY_SCRIPT").c_str();

		/** Interpreter for the shell script */
		const char* shell = tncfhh::Configuration::instance()->get("SHELL").c_str();

		// Prepare the invocation of the shell script
		char* args[4];
		args[0] = const_cast<char*> ( shell );
		args[1] = (recommendation=='Y') ? const_cast<char*> ( allowScript ) : const_cast<char*> ( denyScript );
		args[2] = ip;
		args[3] = 0;

		// Execute the shell script
		execv(args[0], args);

		perror("[PEP daemon] execv() failed");
		exit(0);
	}

	int w;
	wait(&w);
}

#endif
