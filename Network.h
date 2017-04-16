#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <cpr/cpr.h>
#include <memory>
#include <boost/asio.hpp>
#include <iostream>
#include <istream>
#include <ostream>
#include <boost/bind.hpp>


using namespace std;
using boost::asio::ip::tcp;

namespace Crawler
{
	typedef string request_payload;
	typedef string response_payload;

	enum class Request_method {
//		GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS,TRACE
		GET, POST
	};

    std::string make_string(boost::asio::streambuf& sb)
    {
        return {buffers_begin(sb.data()),
                buffers_end(sb.data())};
    };

	class Response
	{
	public:
        unsigned int status_code;
        string header;
		response_payload asio_response;
	};

	class Request
	{
	private:
		string get_request_method(){
			string method_str;
			switch (request_method) {
                case Request_method::GET: method_str = "GET";
				case Request_method::POST: method_str = "POST";
			}
			return method_str;
		}
		string render_request(){
			string request_str;
            request_str = get_request_method() + " " +  resource + " HTTP/1.0\r\n";
            request_str += "Host: " + url + "\r\n";
            request_str += "Accept: */*\r\n";
            request_str += "Connection: close\r\n\r\n";
			return request_str;
		}
	public:
		Request_method request_method;
		string url;
        string resource;
		std::unique_ptr<Response> get() {
			string request_str = this->render_request();
			try {
				boost::asio::io_service io_service;

				// Get a list of endpoints corresponding to the server name.
				tcp::resolver resolver(io_service);
				tcp::resolver::query query(url, "http");
				tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

				// Try each endpoint until we successfully establish a connection.
				tcp::socket socket(io_service);
				boost::asio::connect(socket, endpoint_iterator);

				// Form the request. We specify the "Connection: close" header so that the
				// server will close the socket after transmitting the response. This will
				// allow us to treat all data up until the EOF as the content.
				boost::asio::streambuf request;
				std::ostream request_stream(&request);
				request_stream << request_str;

				// Send the request.
				boost::asio::write(socket, request);

				// Read the response status line. The response streambuf will automatically
				// grow to accommodate the entire line. The growth may be limited by passing
				// a maximum size to the streambuf constructor.
                std::unique_ptr<Response> r = std::make_unique<Response>();

				boost::asio::streambuf response;
				boost::asio::read_until(socket, response, "\r\n");

				// Check that response is OK.
				std::istream response_stream(&response);
				std::string http_version;
				response_stream >> http_version;
				unsigned int status_code;
				response_stream >> status_code;
				std::string status_message;
				std::getline(response_stream, status_message);
				if (!response_stream || http_version.substr(0, 5) != "HTTP/")
				{
					std::cout << "Invalid response\n";
					return nullptr;
				}
				if (status_code != 200)
				{
					std::cout << "Response returned with status code " << status_code << "\n";
					return nullptr;
				}
                r->status_code = status_code;
				// Read the response headers, which are terminated by a blank line.
				boost::asio::read_until(socket, response, "\r\n\r\n");

				// Process the response headers.
				std::string header;
				while (std::getline(response_stream, header) && header != "\r")
					r->header += header + "\n";

				// Write whatever content we already have to output.
				if (response.size() > 0)
					r->asio_response += make_string(response);

				// Read until EOF, writing data to output as we go.
				boost::system::error_code error;
				while (boost::asio::read(socket, response,
										 boost::asio::transfer_at_least(1), error))
					r->asio_response += make_string(response);
				if (error != boost::asio::error::eof)
					throw boost::system::system_error(error);
                return r;
			}
            catch (std::exception& e)
            {
                std::cout << "Exception: " << e.what() << "\n";
                return nullptr;
            }
		}
	};


}