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

    class Authentication {

    private:

        string username;

        string password;

    public:

        Authentication();

        Authentication(string _username,
                       string _password);

        string get_username();

        string get_password();

        void set_username(string _username);

        void set_password(string _password);

        bool operator==(const Authentication& b);

        bool operator!=(const Authentication& b);

    };

    class Response
    {
    public:
        unsigned int status_code;
        string header;
        response_payload asio_response;
        string tag;
        vector<char> content;
    };


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