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
 
#ifndef SSL_CLIENT_H
#define SSL_CLIENT_H

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

namespace tnc {

namespace ssl {

enum {
	max_length = 1024
};


/**
 * Base class for all TLS related client functions. A few
 * parts of the code are based on the examples provides by
 * the boost asio examples - see
 * http://www.boost.org/doc/libs/1_36_0/doc/html/boost_asio/examples.html
 */
class Client {

protected:
	boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;
	char request_[max_length];
	char reply_[max_length];


public:
	/**
	 * Creates a ssl_client and initializes the socket and tries to establish
	 * a connection with the target host.
	 */
	Client(boost::asio::io_service& io_service, boost::asio::ssl::context& context,
	      boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
			: socket_(io_service, context) {

		boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;
		socket_.lowest_layer().connect(endpoint);
		socket_.handshake(boost::asio::ssl::stream_base::client);
	}


	virtual ~Client()
	{
		socket_.lowest_layer().close();
	}
};
} // namespace ssl
} // namespace tnc

#endif
