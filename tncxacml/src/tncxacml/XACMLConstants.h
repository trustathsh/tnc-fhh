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
 
#ifndef XACMLCONSTANTS_H_
#define XACMLCONSTANTS_H_

#include <string>

namespace tncfhh {

namespace iml {

class XACMLConstants {

	public:
	/**
	 *
	 */
	static const std::string attribute_id_subject_id;
	static const std::string attribute_id_component_vendor_id;
	static const std::string attribute_id_component_type;
	static const std::string attribute_id_component_subtype;
	static const std::string attribute_id_attribute_vendor_id;
	static const std::string attribute_id_attribute_type;
	static const std::string attribute_id_product_information_vendor_id;
	static const std::string attribute_id_product_information_product_id;
	static const std::string attribute_id_product_information_product_name;
	static const std::string attribute_id_operational_status_status;
	static const std::string attribute_id_operational_status_result;
	static const std::string attribute_id_operational_status_last_use;
	static const std::string attribute_id_resource_id;
	static const std::string attribute_id_action_id;
	static const std::string attribute_id_attribute_numeric_version_major_version;
	static const std::string attribute_id_attribute_numeric_version_minor_version;
	static const std::string attribute_id_attribute_numeric_version_build_number;
	static const std::string attribute_id_attribute_numeric_version_service_pack_major;
	static const std::string attribute_id_attribute_numeric_version_service_pack_minor;
	static const std::string attribute_id_attribute_string_version_product_version_number;
	static const std::string attribute_id_attribute_string_version_internal_build_number;
	static const std::string attribute_id_attribute_string_version_configuration_version_number;
	static const std::string attribute_id_attribute_installed_packages_packages;
	static const std::string attribute_id_tncs_server_name;
	static const std::string attribute_id_component_recommendation_prefix;
	static const std::string attribute_id_component_recommendation_postfix;

	/**
	 *
	 */
	static const std::string data_type_rfc822_name;
	static const std::string data_type_xml_string;
	static const std::string data_type_xml_date_time;
	static const std::string data_type_xml_integer;

	/**
	 *
	 */
	static const std::string subject_category_tnc_ifm_component;
	static const std::string subject_category_tnc_ifm_attribute_product_information;
	static const std::string subject_category_tnc_ifm_attribute_operational_status;
	static const std::string subject_category_tnc_ifm_attribute_numeric_version;
	static const std::string subject_category_tnc_ifm_attribute_string_version;
	static const std::string subject_category_tnc_ifm_attribute_installed_packages;
	static const std::string subject_category_tnc_tncs;
};

}// namespace iml

} // namespace tncfhh

#endif /* XACMLCONSTANTS_H_ */
