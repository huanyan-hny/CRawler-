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
#include <algorithm>
#include "Util.h"

using namespace std;
using boost::asio::ip::tcp;

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
		string tag;
		vector<char> content;
	};

	class Request
	{
	private:

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

	public:
		Request_method request_method;
		string url;
		string resource;
		Header header;
		Authentication auth;
		bool isFile;
		CookieJar cookiejar = {};
		bool trust_env = true;
		string tag;
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
				case Request_method::GET:
					method_str = "GET";
					break;
				case Request_method::POST: method_str = "POST";
			}
			return method_str;
		}

		string render_request(){
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

        Request(string _request_method, string _url, bool _isFile): request_method(str(_request_method)),
																	isFile(_isFile) {
            int found = _url.find("/");
            url = _url.substr(0,found);
            resource = _url.substr(found);
            cout << url << " " << resource << endl;
        }

        CookieJar& get_cookie_jar() {
            return cookiejar;
        }

        string get_url(){
            return url + resource;
        }
        void set_cookie_jar(const CookieJar& c) {
            cookiejar = c;
        }
	};





}