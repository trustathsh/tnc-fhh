/*
 * rlm_eap_tnc.c    Handles that are called from eap
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
 *   Modifications to integrate with FreeRADIUS configuration
 *   Copyright (C) 2007 Alan DeKok <aland@deployingradius.com>
 */

/*
 * EAP-TNC Packet with EAP Header, general structure
 *
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |      Code     |   Identifier  |            Length             |
 * |               |               |                               |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |      Type     |  Flags  | Ver |          Data Length          |
 * |               |L M S R R| =1  |                               |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |          Data Length          |           Data ...
 * |                               |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

#include <freeradius-devel/autoconf.h>

#include <stdio.h>
#include <stdlib.h>

#include "eap_tnc.h"
#include <naaeap/naaeap.h>
#include <freeradius-devel/rad_assert.h>
//#include <freeradius-devel/libradius.h>

#include <netinet/in.h>

#define VERSION "0.7.0"

/**
 * Calculates an identifying string based upon nas_port, nas_ip and nas_port_type.
 * The maximum length of the calculated string is 70 (not including the trailing '\0').
 *
 * @return the number of bytes written to out (not including the trailing '\0')
 */
static uint32_t calculateConnectionString(RADIUS_PACKET* radius_packet, char *out, size_t outMaxLength)
{
	VALUE_PAIR *vp = NULL;
//	uint32_t outMD5[4]; // 4 x 32 == 128 :-)
//	uint32_t inMD5[3];
	uint32_t nas_port = 0;
	uint32_t nas_ip = 0;
	uint32_t nas_port_type = 0;

	char out_nas_port[11];
	char out_nas_ip_byte_0[4];
	char out_nas_ip_byte_1[4];
	char out_nas_ip_byte_2[4];
	char out_nas_ip_byte_3[4];
	char out_nas_port_type[11];

    // check for NULL
	if (radius_packet == NULL) {
		radlog(L_ERR,
				"rlm_eap_tnc: calculateConnectionString failed. radius_packet == NULL!");
		return 0;
	}

	// read NAS port, ip and port type
	for (vp = radius_packet->vps; vp; vp=vp->next) {
		switch (vp->attribute) {
		case PW_NAS_PORT:
			nas_port = vp->vp_integer;
			DEBUG("NAS scr port = %u\n", nas_port);
			break;
		case PW_NAS_IP_ADDRESS:
			nas_ip = vp->vp_ipaddr;
			DEBUG("NAS scr ip = %X\n", ntohl(nas_ip));
			break;
		case PW_NAS_PORT_TYPE:
			nas_port_type = vp->vp_integer;
			DEBUG("NAS scr port type = %u\n", nas_port_type);
			break;
		}
	}

	// check that all attributes have been set
	// FIXME get values from parent request if possible. does not work as tunneled eap method atm
	// FIXME check the thing above
//	if(nas_ip == 0 ||
//	   nas_port_type == 0){
//		radlog(L_ERR,
//				"rlm_eap_tnc: not all attributes set for calculating connection ID.");
//		return 0;
//	}

	snprintf(out_nas_port, 11, "%u", nas_port);
	snprintf(out_nas_ip_byte_0, 4, "%u", nas_ip & 0xFF);
	snprintf(out_nas_ip_byte_1, 4, "%u", (nas_ip >> 8) & 0xFF);
	snprintf(out_nas_ip_byte_2, 4, "%u", (nas_ip >> 16) & 0xFF);
	snprintf(out_nas_ip_byte_3, 4, "%u", (nas_ip >> 24) & 0xFF);
	snprintf(out_nas_port_type, 11, "%u", nas_port_type);

	return snprintf(out, outMaxLength, "NAS Port: %s NAS IP: %s.%s.%s.%s NAS_PORT_TYPE: %s", out_nas_port, out_nas_ip_byte_3, out_nas_ip_byte_2, out_nas_ip_byte_1, out_nas_ip_byte_0, out_nas_port_type);

//	inMD5[0] = nas_ip;
//	inMD5[1] = nas_port;
//	inMD5[2] = nas_port_type;

	// calculate MD5
//	fr_md5_calc((uint8_t *)outMD5, (uint8_t *)inMD5, sizeof(uint32_t) * 3);

	// take the first 4 byte as conID
//	ret = *((TNC_ConnectionID *)(void *)outMD5);
//	return ret;
}

/*
 * This function is called when the FreeRADIUS attach this module.
 */
static int tnc_attach(CONF_SECTION *conf, void **type_data)
{
    // initialize NAA-EAP
	DEBUG2("TNC-ATTACH initializing NAA-EAP");
	TNC_Result result = initializeDefault();
	if (result != TNC_RESULT_SUCCESS) {
		radlog(L_ERR,
				"rlm_eap_tnc: tnc_attach error while calling NAA-EAP initializeDefault()");
		return -1;
	}
	return 0;
}

/*
 * This function is called when the FreeRADIUS detach this module.
 */
static int tnc_detach(void *args)
{
    // terminate NAA-EAP
	DEBUG2("TNC-TERMINATE terminating NAA-EAP");
	TNC_Result result = terminate();
	if (result != TNC_RESULT_SUCCESS) {
		radlog(L_ERR,
				"rlm_eap_tnc: tnc_attach error while calling NAA-EAP terminate()");
		return -1;
	}
	return 0;
}


/*
 * This function is called when the first EAP_IDENTITY_RESPONSE message
 * was received.
 *
 * Initiates the EPA_TNC session by sending the first EAP_TNC_RESPONSE
 * to the peer. The packet has the Start-Bit set and contains no data.
 *
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |      Code     |   Identifier  |            Length             |
 * |               |               |                               |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |      Type     |  Flags  | Ver |
 * |               |0 0 1 0 0|0 0 1|
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * For this package, only 'Identifier' has to be set dynamically. Any
 * other information is static.
 */
static int tnc_initiate(void *type_data, EAP_HANDLER *handler)
{
	size_t buflen = 71;
	size_t ret = 0;
	char buf[buflen];
	REQUEST * request = NULL;
	TNC_Result result;
	TNC_ConnectionID conID;
	TNC_BufferReference username;

	// check if we run inside a secure EAP method.
	// FIXME check concrete outer EAP method
	if (!handler->request || !handler->request->parent) {
		DEBUG2("rlm_eap_tnc: EAP_TNC must only be used as an inner method within a protected tunneled EAP created by an outer EAP method.");
		request = handler->request;
		return 0;
	} else {
		request = handler->request->parent;
	}

	if (request->packet == NULL) {
		DEBUG2("rlm_eap_tnc: ERROR request->packet is NULL.");
		return 0;
	}

	DEBUG("tnc_initiate: %ld", handler->timestamp);

	//calculate connectionString
	ret = calculateConnectionString(request->packet, buf, buflen);
	if(ret == 0){
		radlog(L_ERR, "rlm_eap_tnc:tnc_attach: calculating connection String failed.");
		return 0;
	}

	DEBUG2("TNC-INITIATE getting connection from NAA-EAP");

	/*
	 * get connection
	 * (uses a function from the NAA-EAP-library)
	 * the presence of the library is checked via the configure-script
	 */
	result = getConnection(buf, &conID);

	// check for errors
	if (result != TNC_RESULT_SUCCESS) {
		radlog(L_ERR,
				"rlm_eap_tnc: tnc_initiate error while calling NAA-EAP getConnection");
		return 0;
	}

	/*
	 * tries to get the username from FreeRADIUS;
	 * copied from modules/rlm_eap/types/rlm_eap_ttls/ttls.c
	 */
	VALUE_PAIR *usernameValuePair;
	usernameValuePair = pairfind(request->packet->vps, PW_USER_NAME);

	VALUE_PAIR *eapMessageValuePair;
	if (!usernameValuePair) {
		eapMessageValuePair = pairfind(request->packet->vps, PW_EAP_MESSAGE);
	
		if (eapMessageValuePair &&
			(eapMessageValuePair->length >= EAP_HEADER_LEN + 2) &&
			(eapMessageValuePair->vp_strvalue[0] == PW_EAP_RESPONSE) &&
			(eapMessageValuePair->vp_strvalue[EAP_HEADER_LEN] == PW_EAP_IDENTITY) &&
			(eapMessageValuePair->vp_strvalue[EAP_HEADER_LEN + 1] != 0)) {

			/*
			 *	Create & remember a User-Name
			 */
			usernameValuePair = pairmake("User-Name", "", T_OP_EQ);
			rad_assert(usernameValuePair != NULL);

			memcpy(usernameValuePair->vp_strvalue, eapMessageValuePair->vp_strvalue + 5,
					eapMessageValuePair->length - 5);
			usernameValuePair->length = eapMessageValuePair->length - 5;
			usernameValuePair->vp_strvalue[usernameValuePair->length] = 0;
		}
	}

	username = malloc(usernameValuePair->length + 1);
	memcpy(username, usernameValuePair->vp_strvalue, usernameValuePair->length);
	username[usernameValuePair->length] = '\0';

	RDEBUG("Username for current TNC connection: %s", username);

	/*
	 * stores the username of this connection
	 * (uses a function from the NAA-EAP-library)
	 * the presence of the library is checked via the configure-script
	 */
	result = storeUsername(conID, username, usernameValuePair->length);

	// check for errors
	if (result != TNC_RESULT_SUCCESS) {
		radlog(L_ERR,
				"rlm_eap_tnc: tnc_initiate error while calling NAA-EAP storeUsername");
		return 0;
	}

	// set connection ID in FreeRADIUS
	handler->opaque = malloc(sizeof(TNC_ConnectionID));
	memcpy(handler->opaque, &conID, sizeof(TNC_ConnectionID));

	// build first EAP TNC request
	TNC_BufferReference eap_tnc_request = malloc(sizeof(unsigned char));
	if (eap_tnc_request == NULL) {
		radlog(L_ERR, "rlm_eap_tnc:tnc_initiate: malloc failed.");
		return 0;
	}
	*eap_tnc_request = SET_START(1);
	TNC_UInt32 eap_tnc_length = 1;
	type_data = type_data; /* suppress -Wunused */

	/*
	 *	Compose the EAP-TNC packet out of the data structure,
	 *	and free it.
	 */
	eaptnc_compose(handler, eap_tnc_request, eap_tnc_length, PW_EAP_REQUEST);

	/*
	 *	We don't need to authorize the user at this point.
	 *
	 *	We also don't need to keep the challenge, as it's
	 *	stored in 'handler->eap_ds', which will be given back
	 *	to us...
	 */
	handler->stage = AUTHENTICATE;

	return 1;
}

/**
 * This function is called when a EAP_TNC_RESPONSE was received.
 * It basically forwards the EAP_TNC data to NAA-TNCS and forms
 * and appropriate EAP_RESPONSE. Furthermore, it sets the VlanID
 * based on the TNC_ConnectionState determined by NAA-TNCS.
 *
 * @param type_arg The configuration data
 * @param handler The EAP_HANDLER
 * @return True, if successfully, else false.
 */
static int tnc_authenticate(void *type_arg, EAP_HANDLER *handler) {

	rad_assert(handler->request != NULL); // check that request has been sent previously
	rad_assert(handler->stage == AUTHENTICATE); // check if initiate has been called

	if (handler == NULL) {
		radlog(L_ERR,
				"rlm_eap_tnc: tnc_authenticate invalid parameters: handler == NULL");
		return 0;
	}
	if (handler->eap_ds == NULL) {
		radlog(L_ERR,
				"rlm_eap_tnc: tnc_authenticate invalid parameters: handler->eap_ds == NULL");
		return 0;
	}
	if (handler->eap_ds->response == NULL) {
		radlog(
				L_ERR,
				"rlm_eap_tnc: tnc_authenticate invalid parameters: handler->eap_ds->resonse == NULL");
		return 0;
	}
	if (handler->eap_ds->response->type.type != PW_EAP_TNC
			|| handler->eap_ds->response->type.length < 1
			|| handler->eap_ds->response->type.data == NULL) {
		radlog(
				L_ERR,
				"rlm_eap_tnc: tnc_authenticate invalid parameters: handler->eap_ds->response->type.type == %X, ->type.length == %u, ->type.data == %p",
				handler->eap_ds->response->type.type,
				handler->eap_ds->response->type.length,
				handler->eap_ds->response->type.data);
		return 0;
	}

	// get connection ID
	TNC_ConnectionID conID = *((TNC_ConnectionID *) (handler->opaque));



	DEBUG2("TNC-AUTHENTICATE is starting now for connection ID %lX !", conID);

	// pass EAP_TNC data to NAA-EAP and get answer data
	TNC_BufferReference output = NULL;
	TNC_UInt32 outputLength = 0;
	TNC_ConnectionState connectionState = TNC_CONNECTION_STATE_CREATE;

	/*
	 * forwards the eap_tnc data to NAA-EAP and gets the response
	 * (uses a function from the NAA-EAP-library)
	 * the presence of the library is checked via the configure-script
	 */
	TNC_Result result = processEAPTNCData(conID, handler->eap_ds->response->type.data,
			handler->eap_ds->response->type.length, &output, &outputLength,
			&connectionState);

	// check for errors
	if (result != TNC_RESULT_SUCCESS) {
		radlog(L_ERR,
				"rlm_eap_tnc: tnc_authenticate error while calling NAA-EAP processEAPTNCData");
		return 0;
	}

	// output contains now the answer from NAA-EAP
	uint8_t eapCode = 0;
	// determine eapCode for request
	switch (connectionState) {
	case TNC_CONNECTION_STATE_HANDSHAKE:
		eapCode = PW_EAP_REQUEST;
		break;
	case TNC_CONNECTION_STATE_ACCESS_NONE:
		eapCode = PW_EAP_FAILURE;
//		pairadd(&handler->request->config_items, pairmake("TNC-Status", "None", T_OP_SET));
		break;
	case TNC_CONNECTION_STATE_ACCESS_ALLOWED:
		eapCode = PW_EAP_SUCCESS;
		pairadd(&handler->request->config_items, pairmake("TNC-Status", "Access", T_OP_SET));
		break;
	case TNC_CONNECTION_STATE_ACCESS_ISOLATED:
		eapCode = PW_EAP_SUCCESS;
		pairadd(&handler->request->config_items, pairmake("TNC-Status", "Isolate", T_OP_SET));
		break;
	default:
		radlog(L_ERR,
				"rlm_eap_tnc: tnc_authenticate invalid TNC_CONNECTION_STATE.");
		return 0;
	}

	// form EAP_REQUEST
	if (!eaptnc_compose(handler, output, outputLength, eapCode)) {
		radlog(L_ERR,
				"rlm_eap_tnc: tnc_authenticate error while forming EAP_REQUEST.");
		return 0;
	}

	// FIXME: Why is that needed?
	handler->stage = AUTHENTICATE;

	return 1;
}

/*
 *	The module name should be the only globally exported symbol.
 *	That is, everything else should be 'static'.
 */
EAP_TYPE rlm_eap_tnc = {
		"eap_tnc",
		tnc_attach, /* attach */
		tnc_initiate, /* Start the initial request */
		NULL, /* authorization */
		tnc_authenticate, /* authentication */
		tnc_detach /* detach */
};
