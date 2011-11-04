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
 
#include "XACMLConstants.h"

namespace tncfhh {

namespace iml {

const std::string XACMLConstants::attribute_id_subject_id = std::string(
		"urn:oasis:names:tc:xacml:1.0:subject:subject-id");
const std::string XACMLConstants::attribute_id_component_vendor_id = std::string(
		"urn:ietf:names:tc:xacml:1.0:component:vendor-id");
const std::string XACMLConstants::attribute_id_component_subtype = std::string(
		"urn:ietf:names:tc:xacml:1.0:component:subtype");
const std::string XACMLConstants::attribute_id_component_type = std::string(
		"urn:ietf:names:tc:xacml:1.0:component:component-type");
const std::string XACMLConstants::attribute_id_attribute_vendor_id = std::string(
		"urn:ietf:names:tc:xacml:1.0:attribute:vendor-id");
const std::string XACMLConstants::attribute_id_attribute_type = std::string(
		"urn:ietf:names:tc:xacml:1.0:attribute:type");
const std::string XACMLConstants::attribute_id_product_information_vendor_id = std::string(
		"urn:ietf:names:tc:xacml:1.0:attribute:product-information:vendor-id");
const std::string XACMLConstants::attribute_id_product_information_product_id = std::string(
		"urn:ietf:names:tc:xacml:1.0:attribute:product-information:product-id");
const std::string XACMLConstants::attribute_id_product_information_product_name = std::string(
		"urn:ietf:names:tc:xacml:1.0:attribute:product-information:product-name");
const std::string XACMLConstants::attribute_id_operational_status_status = std::string(
		"urn:ietf:names:tc:xacml:1.0:attribute:operational-status:status");
const std::string XACMLConstants::attribute_id_operational_status_result = std::string(
		"urn:ietf:names:tc:xacml:1.0:attribute:operational-status:result");
const std::string XACMLConstants::attribute_id_operational_status_last_use = std::string(
		"urn:ietf:names:tc:xacml:1.0:attribute:operational-status:last-use");
const std::string XACMLConstants::attribute_id_resource_id = std::string(
		"urn:oasis:names:tc:xacml:1.0:resource:resource-id");
const std::string XACMLConstants::attribute_id_action_id = std::string(
		"urn:oasis:names:tc:xacml:1.0:resource:action-id");
const std::string XACMLConstants::attribute_id_attribute_numeric_version_major_version = std::string(
		"urn:ietf:names:tc:xacml:1.0:attribute:numeric-version:major-version");
const std::string XACMLConstants::attribute_id_attribute_numeric_version_minor_version = std::string(
		"urn:ietf:names:tc:xacml:1.0:attribute:numeric-version:minor-version");
const std::string XACMLConstants::attribute_id_attribute_numeric_version_build_number = std::string(
		"urn:ietf:names:tc:xacml:1.0:attribute:numeric-version:build-number");
const std::string XACMLConstants::attribute_id_attribute_numeric_version_service_pack_major = std::string(
		"urn:ietf:names:tc:xacml:1.0:attribute:numeric-version:service-pack-major");
const std::string XACMLConstants::attribute_id_attribute_numeric_version_service_pack_minor = std::string(
		"urn:ietf:names:tc:xacml:1.0:attribute:numeric-version:service-pack-minor");
const std::string XACMLConstants::attribute_id_attribute_string_version_product_version_number = std::string(
		"urn:ietf:names:tc:xacml:1.0:attribute:string-version:product-version-number");
const std::string XACMLConstants::attribute_id_attribute_string_version_internal_build_number = std::string(
		"urn:ietf:names:tc:xacml:1.0:attribute:string-version:internal-build-number");
const std::string XACMLConstants::attribute_id_attribute_string_version_configuration_version_number = std::string(
		"urn:ietf:names:tc:xacml:1.0:attribute:string-version:configuration-version-number");
const std::string XACMLConstants::attribute_id_attribute_installed_packages_packages = std::string(
		"urn:ietf:names:tc:xacml:1.0:attribute:installed-packages:packages");
const std::string XACMLConstants::attribute_id_tncs_server_name = std::string(
		"urn:ietf:names:tc:xacml:1.0:tncs:server-name");
const std::string XACMLConstants::attribute_id_component_recommendation_prefix = std::string(
		"tnc:if-m:component:0x000000:");
const std::string XACMLConstants::attribute_id_component_recommendation_postfix = std::string(
		":recommendation");

const std::string XACMLConstants::data_type_rfc822_name = std::string(
		"urn:oasis:names:tc:xacml:1.0:data-type:rfc822Name");
const std::string XACMLConstants::data_type_xml_string = std::string(
		"http://www.w3.org/2001/XMLSchema#string");
const std::string XACMLConstants::data_type_xml_date_time = std::string(
		"http://www.w3.org/2001/XMLSchema#dateTime");
const std::string XACMLConstants::data_type_xml_integer = std::string(
		"http://www.w3.org/2001/XMLSchema#integer");

const std::string XACMLConstants::subject_category_tnc_ifm_component = std::string(
		"tnc:if-m:component");
const std::string XACMLConstants::subject_category_tnc_ifm_attribute_product_information = std::string(
		"tnc:if-m:attribute:product-information");
const std::string XACMLConstants::subject_category_tnc_ifm_attribute_operational_status = std::string(
		"tnc:if-m:attribute:operational-status");
const std::string XACMLConstants::subject_category_tnc_ifm_attribute_numeric_version = std::string(
		"tnc:if-m:attribute:numeric-version");
const std::string XACMLConstants::subject_category_tnc_ifm_attribute_string_version = std::string(
		"tnc:if-m:attribute:string-version");
const std::string XACMLConstants::subject_category_tnc_ifm_attribute_installed_packages = std::string(
		"tnc:if-m:attribute:installed-packages");
const std::string XACMLConstants::subject_category_tnc_tncs = std::string(
		"tnc:tncs");

}// namespace iml

} // namespace tncfhh
