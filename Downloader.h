#pragma once
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "Network.h"
#include <cpr/cpr.h>
#include <curl/curl.h>

using namespace std;
using namespace Crawler;
using boost::asio::ip::tcp;

namespace Crawler
{
	namespace Downloader {
        class Boostasio_Downloader {
        public:
            static shared_ptr<Response> get(Request &req) {
                string request_str = req.render_request();
                try {
                    boost::asio::io_service io_service;

                    // Get a list of endpoints corresponding to the server name.
                    tcp::resolver resolver(io_service);
                    tcp::resolver::query query(req.url, "http");
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
                    if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
                        std::cout << "Invalid response\n";
                        return nullptr;
                    }
                    if (status_code != 200) {
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
//				if (response.size() > 0)
//					r->asio_response += make_string(response);

                    // Read until EOF, writing data to output as we go.
                    boost::system::error_code error;
                    while (boost::asio::read(socket, response,
                                             boost::asio::transfer_at_least(1), error)) {}

                    r->asio_response += make_string(response);
                    if (error != boost::asio::error::eof)
                        throw boost::system::system_error(error);
                    return r;
                }
                catch (std::exception &e) {
                    std::cout << "Exception: " << e.what() << "\n";
                    return nullptr;
                }
            }
            //async
            //std::async ?
            //std::future ?
            //std::thread ?
        };


        class Curl_Downloader {
        public:
            static shared_ptr<Response> get(Request &req) {
                std::shared_ptr<Response> res = std::make_shared<Response>();
                if (req.isFile) {
                    CURL *image;
                    CURLcode imgresult;
                    vector<char> vb;

                    image = curl_easy_init();
                    if( image ){
                        // Open file

                        curl_easy_setopt(image, CURLOPT_URL, req.get_url().c_str());
                        curl_easy_setopt(image, CURLOPT_WRITEFUNCTION, Crawler_Util::write_to_vec);
                        curl_easy_setopt(image, CURLOPT_WRITEDATA, &vb);

                        // Grab image
                        imgresult = curl_easy_perform(image);
                        if( imgresult ){
                            cout << "Cannot grab the image!\n";
                        }
                    }

// Clean up the resources
                    curl_easy_cleanup(image);
// Close the file
                    res->content = vb;

                }

                else {
                    cpr::Response r;
                    if (req.auth.get_username() != ""){
                        r = cpr::Get(cpr::Url{req.url + req.resource}, cpr::Authentication {req.auth.get_username(),req.auth.get_password()});
                        if (r.status_code == 401) // Try sending as both parameters and authentication
                            r = cpr::Get(cpr::Url{req.url + req.resource}, cpr::Parameters {{req.auth.get_username(),req.auth.get_password()}});
                            if (r.status_code == 401)
                                cout << "ERROR: Tried sending authentication both as authentication and parameters, but failed!";
                    }
                    else
                        r = cpr::Get(cpr::Url{req.url + req.resource});
                    res->header = Crawler_Util::cpr_header_to_string(r.header);
                    res->status_code = r.status_code;
                    res->asio_response = r.text;
                }
                return res;
            }
        };
    }


    class Session
    {
    private:
        Header header;
        Authentication auth;
        CookieJar cookiejar;
        bool trust_env = true;

    public:
        Session(){}
        template <typename... Tail>
        shared_ptr<Response> request(string method, string url, bool isFile, Tail... tail)
        {
            Request r {method, url, Crawler::Request_content::STRING,""};
            Crawler_Util::set_option(r, tail...);
            merge_cookie(r.get_cookie_jar(), cookiejar);
            std::shared_ptr<Response> res = Crawler::Downloader::Boostasio_Downloader::get(r);
            extract_cookie_to_cookiejar(cookiejar, res->header);
            return res;
        }
        template <typename... Tail>
        shared_ptr<Response> get(string url, bool isFile, Tail... tail)
        {
            return request("GET", url,  isFile, tail...);
        }
    };
}