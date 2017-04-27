#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <mutex>
#include <cpr/cpr.h>
#include <curl/curl.h>
#include "Task.h"
#include "Network.h"
#include "Downloader.h"



namespace Crawler
{
    Session::Session() {};
    Session::Session(const Session &obj) {
        // copy constructor
        cookiejar = obj.cookiejar;
    }


    namespace Downloader {
        shared_ptr<Response> Boostasio_Downloader::get(Task &tsk) {
            Request req = Request(tsk.get_method(), tsk.get_url(), tsk.get_content());
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
//              if (response.size() > 0)
//                  r->asio_response += make_string(response);

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
    }; // namespace Downloader


    shared_ptr<Response> Downloader::Curl_Downloader::post(Task &tsk) {

        shared_ptr<Session> s;

        cpr::Payload pl {};
        for (auto &k: tsk.get_form()) {
            pl.AddPair({k.first, k.second});
        }

        if ( tsk.get_session_type() == Crawler::Session_type::NEW ){
            s = std::make_shared<Session>();
            sm.insert({tsk.get_session_name(), s});
        }
        else if ( tsk.get_session_type() == Crawler::Session_type::DEFAULT ) {
            s = std::make_shared<Session>();
        }
        else {
            auto session_iter = sm.find(tsk.get_session_name());

            if ( session_iter == sm.end() ) {
                if ( tsk.get_session_type() == Crawler::Session_type::SPECIFIED || tsk.get_session_type() == Crawler::Session_type::AUTO)
                    std::cout << "WARNING: Session " << tsk.get_session_name() << " is not found! Used default session instead" << endl;
                s = std::make_shared<Session>();
            }
            else
                s = session_iter->second;
        }
        Session session_cpy = *s.get(); // a copy
        std::shared_ptr<Response> res = std::make_shared<Response>();
        if (tsk.get_content() == Request_content::FILE) {
            CURL *file;
            CURLcode fileresult;
            vector<char> vb;

            file = curl_easy_init();
            if( file ){
                // Open file

                curl_easy_setopt(file, CURLOPT_URL, tsk.get_url().c_str());
                curl_easy_setopt(file, CURLOPT_WRITEFUNCTION, Crawler_Util::write_to_vec);
                curl_easy_setopt(file, CURLOPT_WRITEDATA, &vb);
                curl_easy_setopt(file, CURLOPT_SSL_VERIFYPEER, 0L);
                // Grab image
                fileresult = curl_easy_perform(file);
                if( fileresult ){
                    cout << "Cannot grab the image!\n";
                }
            }
            // Clean up the resources
            curl_easy_cleanup(file);
            // Close the file
            res->content = vb;

        }

        else {
            cpr::Response r;
            session_cpy.session.SetHeader(tsk.get_header());
            if (tsk.get_auth_type() != Auth_type::NONE){
                session_cpy.session.SetUrl(cpr::Url{tsk.get_url()});
                if (tsk.get_auth_type() == Auth_type::BASIC)
                    session_cpy.session.SetAuth(cpr::Authentication {tsk.get_authentication().get_username(),tsk.get_authentication().get_password()});
                else
                    session_cpy.session.SetHeader({{"Authorization", tsk.get_authentication().get_username() + " " + tsk.get_authentication().get_password()}});
                session_cpy.session.SetCookies(session_cpy.cookiejar);
                session_cpy.session.SetPayload(pl);
                r = session_cpy.session.Post();
                if (r.status_code == 401) {// Try sending as both parameters and authentication
                    session_cpy.session.SetUrl(cpr::Url{tsk.get_url()});
                    session_cpy.session.SetAuth(cpr::Authentication{"",""});
                    session_cpy.session.SetCookies(session_cpy.cookiejar);
                    session_cpy.session.SetParameters(cpr::Parameters {{tsk.get_authentication().get_username(),tsk.get_authentication().get_password()}});
                    session_cpy.session.SetPayload(pl);
                    r = session_cpy.session.Post();
                }
                if (r.status_code == 401)
                    cout << "ERROR: Tried sending authentication both as authentication and parameters, but failed!";
            }
            else {

                session_cpy.session.SetUrl(cpr::Url{tsk.get_url()});
                session_cpy.session.SetCookies(session_cpy.cookiejar);
                session_cpy.session.SetPayload(pl);
                r = session_cpy.session.Post();
            }
            res->header = Crawler_Util::cpr_header_to_string(r.header);
            res->status_code = r.status_code;
            res->asio_response = r.text;
            s->cookiejar_mtx.lock();
            s->cookiejar = r.cookies;
            s->cookiejar_mtx.unlock();
        }
        return res;

    }

    shared_ptr<Response> Downloader::Curl_Downloader::get(Task &tsk) {

        shared_ptr<Session> s;

        if ( tsk.get_session_type() == Crawler::Session_type::NEW ){
            s = std::make_shared<Session>();
            sm.insert({tsk.get_session_name(), s});
        }
        else if ( tsk.get_session_type() == Crawler::Session_type::DEFAULT ) {
            s = std::make_shared<Session>();
        }
        else {
            auto session_iter = sm.find(tsk.get_session_name());

            if ( session_iter == sm.end() ) {
                if ( tsk.get_session_type() == Crawler::Session_type::SPECIFIED || tsk.get_session_type() == Crawler::Session_type::AUTO)
                    std::cout << "WARNING: Session " << tsk.get_session_name() << " is not found! Used default session instead" << endl;
                s = std::make_shared<Session>();
            }
            else
                s = session_iter->second;
        }
        Session session_cpy = *s.get(); // a copy
        std::shared_ptr<Response> res = std::make_shared<Response>();
        if (tsk.get_content() == Request_content::FILE) {
            CURL *file;
            CURLcode fileresult;
            vector<char> vb;

            file = curl_easy_init();
            if( file ){
                // Open file

                curl_easy_setopt(file, CURLOPT_URL, tsk.get_url().c_str());
                curl_easy_setopt(file, CURLOPT_WRITEFUNCTION, Crawler_Util::write_to_vec);
                curl_easy_setopt(file, CURLOPT_WRITEDATA, &vb);
                curl_easy_setopt(file, CURLOPT_SSL_VERIFYPEER, 0L);
                // Grab image
                fileresult = curl_easy_perform(file);
                if( fileresult ){
                    cout << "Cannot grab the image!\n";
                }
            }
            // Clean up the resources
            curl_easy_cleanup(file);
            // Close the file
            res->content = vb;

        }

        else {
            cpr::Response r;
            session_cpy.session.SetHeader(tsk.get_header());
            if (tsk.get_auth_type() != Auth_type::NONE){
                session_cpy.session.SetUrl(cpr::Url{tsk.get_url()});
                if (tsk.get_auth_type() == Auth_type::BASIC)
                    session_cpy.session.SetAuth(cpr::Authentication {tsk.get_authentication().get_username(),tsk.get_authentication().get_password()});
                else
                    session_cpy.session.SetHeader({{"Authorization", tsk.get_authentication().get_username() + " " + tsk.get_authentication().get_password()}});
                session_cpy.session.SetCookies(session_cpy.cookiejar);
                r = session_cpy.session.Get();
                if (r.status_code == 401) {// Try sending as both parameters and authentication
                    session_cpy.session.SetUrl(cpr::Url{tsk.get_url()});
                    session_cpy.session.SetAuth(cpr::Authentication{"",""});
                    session_cpy.session.SetCookies(session_cpy.cookiejar);
                    session_cpy.session.SetParameters(cpr::Parameters {{tsk.get_authentication().get_username(),tsk.get_authentication().get_password()}});
                    r = session_cpy.session.Get();
                }
                if (r.status_code == 401)
                    cout << "ERROR: Tried sending authentication both as authentication and parameters, but failed!";
            }
            else {

                session_cpy.session.SetUrl(cpr::Url{tsk.get_url()});
                session_cpy.session.SetCookies(session_cpy.cookiejar);
                r = session_cpy.session.Get();
            }
            res->header = Crawler_Util::cpr_header_to_string(r.header);
            res->status_code = r.status_code;
            res->asio_response = r.text;
            s->cookiejar_mtx.lock();
            s->cookiejar = r.cookies;
            s->cookiejar_mtx.unlock();
        }
        return res;
    }
}; // namespace crawler
