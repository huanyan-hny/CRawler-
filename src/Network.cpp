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
#include "include/CRawlerPlusPlus/Util.h"
#include "include/CRawlerPlusPlus/Network.h"

using namespace std;
using boost::asio::ip::tcp;

namespace Crawler
{


    void extract_cookie_to_cookiejar(CookieJar& ckj, string header) {
        vector<string> headerLines = Crawler_Util::split(header, '\n');

        for (int i(0); i != headerLines.size(); ++i) {
            if (headerLines[i].find("Set-Cookie:") != std::string::npos) {
                std::string variablesPart = Crawler_Util::split(Crawler_Util::split(headerLines[i], ';')[0], ':')[1];
                cout << variablesPart;
                ckj.insert({Crawler_Util::split(variablesPart,'=')[0], Crawler_Util::split(variablesPart,'=')[1]});
            }
        }
    }

    void merge_cookie(Crawler::CookieJar& c1, Crawler::CookieJar c2) {
        c2.insert(c1.begin(),c1.end());
        c1 = c2;
    }

    std::string make_string(boost::asio::streambuf& sb)
    {
        return {buffers_begin(sb.data()),
                buffers_end(sb.data())};
    };

    Authentication::Authentication(): username(""), password("") {}
    Authentication::Authentication(string _username,
                                   string _password):
            username(_username), password(_password){}

    string Authentication::get_username() {
        return username;
    }

    string Authentication::get_password() {
        return password;
    }

    void Authentication::set_username(string _username) {
        username = _username;
    }

    void Authentication::set_password(string _password) {
        password = _password;
    }

    bool Authentication::operator==(const Authentication& b) {
        return username == b.username && password == b.password;
    }

    bool Authentication::operator!=(const Authentication& b) {
        return !(*this == b);
    }



    void Request::set_option(string _tag) {
        this->tag = _tag;
    }

    void Request::set_option(Header header) {
        this->header = header;
    }

    void Request::set_option(Authentication auth){
        this->auth = auth;
    }

    void Request::set_option(bool trust_env) {
        this->trust_env = trust_env;
    }

    Request_method Request::str(string method_str) {
        string str_to_match = Crawler_Util::to_lower(method_str);
        if (str_to_match == "get")
            return Request_method::GET;
        else if (str_to_match == "post")
            return Request_method::POST;
        else
            return Request_method::GET;
    }

    Request& Request::put_string(string key,string value) {
        bundle[key] = value;
        return *this;
    }

    Request& Request::ignore_iteration_limit(bool value) {
        ignore_iterating_limit = value;
        return *this;
    }

    string Request::get_request_method(){
        string method_str;
        switch (request_method) {
            case Request_method::GET:
                method_str = "GET";
                break;
            case Request_method::POST: method_str = "POST";
        }
        return method_str;
    }

    string Request::render_request(){
        string request_str;
        request_str = get_request_method() + " " +  resource + " HTTP/1.0\r\n";
        request_str += "Host: " + url + "\r\n";
        request_str += "Accept: */*\r\n";
        request_str += "Connection: close\r\n";
        if (cookiejar.size()) {
            request_str += "Cookie:";
            for (auto const & s: cookiejar){
                request_str += s.first + "=" + s.second + ";";
            }
        }
        request_str += "\r\n";
        cout << request_str;
        return request_str;
    }


    Request::Request(string _request_method, string _url, Request_content content, string _tag): request_method(str(_request_method))
    {
        if (content == Request_content::FILE)
            isFile = true;
        else
            isFile = false;
        tag = _tag;
        int found = _url.find("/");
        url = _url.substr(0,found);
        resource = _url.substr(found);
        ignore_iterating_limit = false;
        //            cout << url + resource << endl;
    }


    Request::Request(Request_method _request_method, string _url, Request_content content, string _tag): request_method(_request_method)
    {
        if (content == Request_content::FILE)
            isFile = true;
        else
            isFile = false;
        tag = _tag;
        int found = _url.find("/");
        url = _url.substr(0,found);
        resource = _url.substr(found);
        ignore_iterating_limit = false;
        //            cout << url + resource << endl;
    }

    CookieJar& Request::get_cookie_jar() {
        return cookiejar;
    }

    string Request::get_url(){
        return url + resource;
    }

    void Request::set_cookie_jar(const CookieJar& c) {
        cookiejar = c;
    }





}; //namespace crawler