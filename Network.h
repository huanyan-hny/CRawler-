#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <memory>
#include <boost/asio.hpp>
#include <iostream>
#include <istream>
#include <ostream>
#include <boost/bind.hpp>
#include <algorithm>
#include "Util.h"


using namespace std;
using boost::asio::ip::tcp;


namespace Not_Crawler
{
	class Request
	{
	public:
		string payload;
	};
	class Response
	{
	public:
		string payload;
	};

}


//TODO: replace the current mocking req, res with the actual ones

namespace Crawler
{
	typedef string request_payload;
	typedef string response_payload;
    typedef std::unordered_map<std::string,std::string> Header;
    typedef std::unordered_map<std::string,std::string> CookieJar;

	enum class Request_method {
//		GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS,TRACE
		GET, POST
	};

    void merge_cookie(Crawler::CookieJar& c1, Crawler::CookieJar c2) {
        c2.insert(c1.begin(),c1.end());
        c1 = c2;
    }

    std::string make_string(boost::asio::streambuf& sb)
    {
        return {buffers_begin(sb.data()),
                buffers_end(sb.data())};
    };

    class Authentication
    {
    private:
        string username;
        string password;
    public:
        Authentication(): username(""), password("") {}
        Authentication(string _username,
                       string _password):
                username(_username), password(_password){}

        bool operator==(const Authentication& b) {
            return username == b.username && password == b.password;
        }

        bool operator!=(const Authentication& b) {
            return !(*this == b);
        }



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
        Request_method request_method;
        string url;
        string resource;
        Header header;
        Authentication auth;
        CookieJar cookiejar = {};
        bool trust_env = true;

        void set_option(string _request_method) {
            this->request_method = str(_request_method);
        }

        void set_option(Header header) {
            this->header = header;
        }

        void set_option(Authentication auth){
            this->auth = auth;
        }

        void set_option(bool trust_env) {
            this->trust_env = trust_env;
        }

        Request_method str(string method_str) {
            string str_to_match = Crawler_Util::to_lower(method_str);
            if (str_to_match == "get")
                return Request_method::GET;
            else if (str_to_match == "post")
                return Request_method::POST;
            else
                return Request_method::GET;
        }
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
            request_str = get_request_method();
            request_str += " " ;
            request_str += resource ;
            request_str += " HTTP/1.0\r\n";
            request_str += "Host: " + url + "\r\n";
            request_str += "Accept: */*\r\n";
            request_str += "Connection: close\r\n\r\n";
			return request_str;
		}
	public:

        Request(string _request_method, string _url):request_method(str(_request_method)){
            int found = _url.find("/");
            url = _url.substr(0,found);
            resource = _url.substr(found);
            cout << url << " " << resource << endl;
        }

        CookieJar& get_cookie_jar() {
            return cookiejar;
        }

        void set_cookie_jar(const CookieJar& c) {
            cookiejar = c;
        }

		std::shared_ptr<Response> get() {
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
                std::shared_ptr<Response> r = std::make_shared<Response>();

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



    class Session
    {
    private:
        Header header;
        Authentication auth;
        CookieJar const cookiejar;
        bool trust_env = true;

    public:
        Session(){}
        template <typename... Tail>
        shared_ptr<Response> request(string method, string url, Tail... tail)
        {
            Request r {method, url};
            Crawler_Util::set_option(r, tail...);
            merge_cookie(r.get_cookie_jar(), cookiejar);
            return r.get();
        }
        template <typename... Tail>
        shared_ptr<Response> get(string url, Tail... tail)
        {
            return request("GET", url,  tail...);
        }
    };

}