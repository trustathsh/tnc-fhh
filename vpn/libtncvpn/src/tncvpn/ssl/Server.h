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
 
#ifndef SERVER_H
#define SERVER_H

#include <tncutil/Configuration.h>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

namespace tnc {

namespace ssl {

/**
 * Handles incoming SSL request and redirects them
 * to the tnc_session object.
 */
class Server {

protected:
    boost::asio::io_service& io_service_;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ssl::context context_;

    /**
     * SSL password callback, just a dummy password. The certificates
     * are not intended to be used in production environments since
     * they are taken from the boost examples.
     */
    std::string getPassword() const {
      return tncfhh::Configuration::instance()->get("SERVER_KEY_PASSWORD");
    }

public:

	/**
	 * Initializes the TLS context (including the certificates)
	 */
	Server(boost::asio::io_service& io_service, unsigned short port) :
		io_service_(io_service),
		acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
		context_(io_service, boost::asio::ssl::context::sslv23)
	{
		// Initializing the SSL context object
		context_.set_options(
				boost::asio::ssl::context::default_workarounds
				| boost::asio::ssl::context::no_sslv2
				| boost::asio::ssl::context::single_dh_use);

		context_.set_password_callback(boost::bind(&Server::getPassword, this));
		context_.use_certificate_chain_file(tncfhh::Configuration::instance()->get("SERVER_KEY"));
		context_.use_private_key_file(tncfhh::Configuration::instance()->get("SERVER_KEY"), boost::asio::ssl::context::pem);
		context_.use_tmp_dh_file(tncfhh::Configuration::instance()->get("DH_FILE"));
	}

};

} // namespace ssl

} // namespace tnc

#endif
