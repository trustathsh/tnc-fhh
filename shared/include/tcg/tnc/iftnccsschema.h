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
 
/** @file
 *
 * TNCCS schema represented as const char*.
 *
 * <h3>Changelog:</h3>
 * <ul>
 *   <li>04.03.2008 - create file (mbs)</li>
 * </ul>
 *
 * @date 04.03.2008
 * @author: Mike Steinmetz (mbs)
 */
#ifndef TNC_IFTNCCSSCHEMA_H_
#define TNC_IFTNCCSSCHEMA_H_

#define TNC_IF_TNCCS_SCHEMA \
	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" \
	"<xs:schema\n" \
	"	xmlns=\"http://www.trustedcomputinggroup.org/IWG/TNC/1_0/IF_TNCCS#\"\n" \
	"	xmlns:xs=\"http://www.w3.org/2001/XMLSchema\"\n" \
	"	targetNamespace=\"http://www.trustedcomputinggroup.org/IWG/TNC/1_0/IF_TNCCS#\"\n" \
	"	elementFormDefault=\"qualified\" attributeFormDefault=\"unqualified\" version=\"1.0\"\n" \
	"	id=\"TNCCS-Batch\">\n" \
	"\n" \
	"<xs:element name=\"TNCCS-Batch\">\n" \
	"	<xs:complexType>\n" \
	"		<xs:sequence>\n" \
	"			<xs:element name=\"TNCC-TNCS-Message\" type=\"TNCC-TNCS-Message\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>\n" \
	"			<xs:element name=\"IMC-IMV-Message\" type=\"IMC-IMV-Message\" minOccurs=\"0\" maxOccurs=\"unbounded\"/>\n" \
	"		</xs:sequence>\n" \
	"		<xs:attribute name=\"BatchId\" type=\"xs:nonNegativeInteger\" use=\"required\"></xs:attribute>\n" \
	"		<xs:attribute name=\"Recipient\" type=\"TNCCS-Recipient-Type\" use=\"required\"></xs:attribute>\n" \
	"	</xs:complexType>\n" \
	"</xs:element>\n" \
	"\n" \
	"<xs:complexType name=\"TNCC-TNCS-Message\">\n" \
	"	<xs:sequence>\n" \
	"		<xs:element name=\"Type\" type=\"TNCCS-Message-Type\"/>\n" \
	"		<xs:choice>\n" \
	"			<xs:element name=\"Base64\" type=\"xs:base64Binary\"/>\n" \
	"			<xs:element name=\"XML\" type=\"XML\"/>\n" \
	"		</xs:choice>\n" \
	"	</xs:sequence>\n" \
	"</xs:complexType>\n" \
	"\n" \
	"<xs:complexType name=\"IMC-IMV-Message\">\n" \
	"	<xs:sequence>\n" \
	"		<xs:element name=\"Type\" type=\"TNCCS-Message-Type\"/>\n" \
	"		<xs:element name=\"Base64\" type=\"xs:base64Binary\"/>\n" \
	"	</xs:sequence>\n" \
	"</xs:complexType>\n" \
	"\n" \
	"<xs:complexType name=\"XML\">\n" \
	"	<xs:sequence>\n" \
	"		<xs:any namespace=\"##any\" processContents=\"lax\"/>\n" \
	"	</xs:sequence>\n" \
	"</xs:complexType>\n" \
	"\n" \
	"<xs:simpleType name=\"TNCCS-Message-Type\">\n" \
	"	<xs:restriction base=\"xs:hexBinary\">\n" \
	"		<xs:minLength value=\"4\"/>\n" \
	"		<xs:maxLength value=\"4\"/>\n" \
	"	</xs:restriction>\n" \
	"</xs:simpleType>\n" \
	"\n" \
	"<xs:simpleType name=\"TNCCS-Recipient-Type\">\n" \
	"	<xs:restriction base=\"xs:NMTOKENS\">\n" \
	"		<xs:enumeration value=\"TNCS\"/>\n" \
	"		<xs:enumeration value=\"TNCC\"/>\n" \
	"	</xs:restriction>\n" \
	"</xs:simpleType>\n" \
	"\n" \
	"<xs:simpleType name=\"TNCCS-Recommendations\">\n" \
	"	<xs:restriction base=\"xs:NMTOKENS\">\n" \
	"		<xs:enumeration value=\"allow\"/>\n" \
	"		<xs:enumeration value=\"none\"/>\n" \
	"		<xs:enumeration value=\"isolate\"/>\n" \
	"	</xs:restriction>\n" \
	"</xs:simpleType>\n" \
	"\n" \
	"<xs:simpleType name=\"TNCCS-Recommendation-Message-Type\">\n" \
	"	<xs:restriction base=\"TNCCS-Message-Type\">\n" \
	"		<xs:enumeration value=\"00000001\"/>\n" \
	"	</xs:restriction>\n" \
	"</xs:simpleType>\n" \
	"\n" \
	"<xs:complexType name=\"TNCCS-Recommendation\">\n" \
	"	<xs:attribute name=\"type\" type=\"TNCCS-Recommendations\"/>\n" \
	"</xs:complexType>\n" \
	"\n" \
	"<xs:element name=\"TNCCS-Recommendation\" type=\"TNCCS-Recommendation\"/>\n" \
	"\n" \
	"<xs:simpleType name=\"TNCCS-Errors\">\n" \
	"	<xs:restriction base=\"xs:NMTOKENS\">\n" \
	"		<xs:enumeration value=\"batch-too-long\"/>\n" \
	"		<xs:enumeration value=\"malformed-batch\"/>\n" \
	"		<xs:enumeration value=\"invalid-batch-id\"/>\n" \
	"		<xs:enumeration value=\"invalid-recipient-type\"/>\n" \
	"		<xs:enumeration value=\"internal-error\"/>\n" \
	"		<xs:enumeration value=\"other\"/>\n" \
	"	</xs:restriction>\n" \
	"</xs:simpleType>\n" \
	"\n" \
	"<xs:simpleType name=\"TNCCS-Error-Message-Type\">\n" \
	"	<xs:restriction base=\"TNCCS-Message-Type\">\n" \
	"		<xs:enumeration value=\"00000002\"/>\n" \
	"	</xs:restriction>\n" \
	"</xs:simpleType>\n" \
	"\n" \
	"<xs:complexType name=\"TNCCS-Error\">\n" \
	"	<xs:attribute name=\"type\" type=\"TNCCS-Errors\"/>\n" \
	"</xs:complexType>\n" \
	"\n" \
	"<xs:element name=\"TNCCS-Error\" type=\"TNCCS-Error\"/>\n" \
	"\n" \
	"<xs:simpleType name=\"TNCCS-PreferredLanguage-Message-Type\">\n" \
	"	<xs:restriction base=\"TNCCS-Message-Type\">\n" \
	"		<xs:enumeration value=\"00000003\" />\n" \
	"	</xs:restriction>\n" \
	"</xs:simpleType>\n" \
	"\n" \
	"<xs:element name=\"TNCCS-PreferredLanguage\" type=\"xs:string\"></xs:element>\n" \
	"\n" \
	"<xs:simpleType name=\"TNCCS-ReasonStrings-Message-Type\">\n" \
	"	<xs:restriction base=\"TNCCS-Message-Type\">\n" \
	"		<xs:enumeration value=\"00000004\" />\n" \
	"	</xs:restriction>\n" \
	"</xs:simpleType>\n" \
	"\n" \
	"<xs:complexType name=\"TNCCS-ReasonStrings\">\n" \
	"	<xs:sequence>\n" \
	"		<xs:element name=\"ReasonString\" type=\"xs:string\" minOccurs=\"1\" maxOccurs=\"unbounded\" />\n" \
	"	</xs:sequence>\n" \
	"</xs:complexType>\n" \
	"\n" \
	"<xs:element name=\"TNCCS-ReasonStrings\" type=\"TNCCS-ReasonStrings\"></xs:element>\n" \
	"\n" \
	"</xs:schema>\n"

#endif /*TNC_IFTNCCSSCHEMA_H_*/
