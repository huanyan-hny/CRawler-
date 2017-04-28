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
#include <cpr/cpr.h>
#include "Util.h"

using namespace std;
using boost::asio::ip::tcp;

namespace Crawler
{
    typedef string request_payload;
    typedef string response_payload;
    typedef std::map<std::string,std::string> CookieJar;
    using Header = cpr::Header;

    enum class Request_method {
//		GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE
                GET, POST
    };

    enum class Request_content {
        FILE,
        HTML,
        JSON,
    };

    void extract_cookie_to_cookiejar(CookieJar& ckj, string header);

    void merge_cookie(Crawler::CookieJar& c1, Crawler::CookieJar c2);

    std::string make_string(boost::asio::streambuf& sb);

    /// Authentication class is the object that contains username/password info used when sending
    /// auth in request
    class Authentication {

    private:

        string username; ///< username, or auth type in OAUTH2.0 (such as "BEARER")

        string password; ///< password, or token in OAUTH2.0

    public:

        Authentication();

        ///
        /// \param _username a string for username
        /// \param _password a string for password
        Authentication(string _username,
                       string _password);

        ///
        /// \return return private member username
        string get_username();

        ///
        /// \return return private member password
        string get_password();

        ///
        /// \param _username set private member username
        void set_username(string _username);

        ///
        /// \param _username set private member password
        void set_password(string _password);

        /// We overload equality check for user to easily compare to authentication.
        /// \param b A constant reference to another authentication
        /// \return boolean (true or false)
        bool operator==(const Authentication& b);

        /// We overload inequality check for user to easily compare to authentication.
        /// \param b A constant reference to another authentication
        /// \return boolean (true or false)
        bool operator!=(const Authentication& b);

    };

    /// Response class is what being returned from Downloader.get or Downloader.post.
    class Response
    {
    public:

        unsigned int status_code; ///< status_code 401/200/404, etc.
        string header; ///< the header string
        response_payload asio_response; ///< the content of HTML/JSON.
        string tag; ///< the tag information got from Task/Request (if any). Deprecated.
        vector<char> content; ///< content vector char for files like image/pdf.
    };


    /// Request class is deprecated. Now use Task for everywhere!
    class Request
    {
    private:


    public:
        void set_option(string _tag);

        void set_option(Header header);

        void set_option(Authentication auth);

        void set_option(bool trust_env);

        Request_method request_method;

        string url;

        string resource;

        unordered_map<string,string> bundle;

        Header header;

        Authentication auth;

        bool isFile;

        bool ignore_iterating_limit;

        CookieJar cookiejar = {};

        bool trust_env = true;

        string tag;

        Request_method str(string method_str);

        Request& put_string(string key,string value);

        Request& ignore_iteration_limit(bool value);

        string get_request_method();

        string render_request();

        ///
        /// \tparam Tail Variadic template that would take anything.
        /// \param _request_method GET/POST
        /// \param _url url string
        /// \param content FILE/JSON/HTML
        /// \param tail other setting
        template <typename... Tail>
        Request(string _request_method, string _url, Request_content content, Tail... tail): request_method(str(_request_method))
        {
            if (content == Request_content::FILE)
                isFile = true;
            else
                isFile = false;
            int found = _url.find("/");
            url = _url.substr(0,found);
            resource = _url.substr(found);
            ignore_iterating_limit = false;
//            cout << url + resource << endl;
            Crawler_Util::set_option(*this, tail...);
        }

        Request(string _request_method, string _url, Request_content content, string _tag = "");

        Request(Request_method _request_method, string _url, Request_content content, string _tag = "");

        CookieJar& get_cookie_jar();

        string get_url();

        void set_cookie_jar(const CookieJar& c);
    };





}