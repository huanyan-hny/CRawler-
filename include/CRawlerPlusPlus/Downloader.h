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
#include "../../Task.h"
#include <mutex>

using namespace std;
using namespace Crawler;
using boost::asio::ip::tcp;

namespace Crawler {

    struct Session {
        cpr::Session session;
        cpr::Cookies cookiejar;
        mutex cookiejar_mtx;
        Session();
        Session(const Session &obj);
    };

    typedef std::map<string,shared_ptr<Session>> SessionMap;

	namespace Downloader {
        class Boostasio_Downloader {
            public:
                shared_ptr<Response> get(Task &tsk);
        };


        class Curl_Downloader {
            private:
                SessionMap sm;
            public:
                shared_ptr<Response> post(Task &tsk);
                shared_ptr<Response> get(Task &tsk);
        };
    }

}