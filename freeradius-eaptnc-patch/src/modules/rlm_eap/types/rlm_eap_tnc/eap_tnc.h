/*
 *   This software is Copyright (C) 2006-2009 FH Hannover
 *
 *   Portions of this code unrelated to FreeRADIUS are available
 *   separately under a commercial license.  If you require an
 *   implementation of EAP-TNC that is not under the GPLv2, please
 *   contact trust@f4-i.fh-hannover.de for details.
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 *
 */

#ifndef _EAP_TNC_H
#define _EAP_TNC_H

#include "eap.h"
#include <naaeap/naaeap.h>

#define SET_START(x) 		((x) | (0x20))

/**
 * Composes the EAP packet.
 *
 * @param handler The EAP_HANDLER from tnc_initiate() or tnc_authenticate
 * @param request The EAP_TNC packet received from NAA-TNCS
 * @param length The length of the EAP_TNC packet received from NAA-TNCS
 * @param code EAP_CODE for the request
 *
 * @return True if operation was successful, otherwise false.
 */
int eaptnc_compose(EAP_HANDLER *handler, TNC_BufferReference request, TNC_UInt32 length, uint8_t code);

#endif /*_EAP_TNC_H*/
