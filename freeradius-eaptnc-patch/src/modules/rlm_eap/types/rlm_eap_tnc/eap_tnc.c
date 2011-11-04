/*
 *   eap_tnc.c  EAP TNC functionality.
 *
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
#include <stdio.h>
#include <stdlib.h>
#include "eap.h"

#include "eap_tnc.h"

/*
 *	Forms an EAP_REQUEST packet from the EAP_TNC specific data.
 */
int eaptnc_compose(EAP_HANDLER *handler, TNC_BufferReference request, TNC_UInt32 length, uint8_t code)
{
	// check parameters
	if(handler == NULL || (request == NULL && length != 0) || (request != NULL && length < 1) || code > PW_EAP_MAX_CODES){
		radlog(L_ERR, "rlm_eap_tnc: eaptnc_compose invalid parameters: handler == %p, request == %p, length == %lu, code == %u", handler, request, length, code);
		return 0;
	}
	
	// further check parameters
	if(handler->opaque == NULL || handler->eap_ds == NULL){
		radlog(L_ERR, "rlm_eap_tnc: eaptnc_compose invalid parameters: handler->opaque == %p, handler->eap_ds == %p", handler->opaque, handler->eap_ds);
		return 0;
	}

	if(handler->eap_ds->request == NULL){
		radlog(L_ERR, "rlm_eap_tnc: eaptnc_compose invalid parameters: handler->eap_ds->request == %p", handler->eap_ds->request);
		return 0;
	}

	// fill EAP data to handler
	handler->eap_ds->request->code = code;
	handler->eap_ds->request->type.type = PW_EAP_TNC;
	// fill EAP TYPE specific data to handler
	handler->eap_ds->request->type.length = length;
	free(handler->eap_ds->request->type.data);
	handler->eap_ds->request->type.data = request;
	
	return 1;
}
