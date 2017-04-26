//
// Created by elega on 4/25/17.
//

#ifndef CRAWLERPLUSPLUS_GIT_TASK_H
#define CRAWLERPLUSPLUS_GIT_TASK_H

#include <unordered_map>
#include <functional>
#include <string>
#include "Network.h"
using namespace std;

namespace Crawler {


    enum class Session_type {
        NEW,
        LAST,
        AUTO,
        SPECIFIED,
        DEFAULT,
    };

    class Item;
    class Task;
    typedef function<void(shared_ptr<Task>,shared_ptr<Response>,function<void(Task&)>,function<void(Item&)> )>
            spider_callback;
    class Task {
    public:
        Task(string _url,
             spider_callback _callback,
             Request_method _method = Request_method::GET, Request_content _content = Request_content::STRING,
             Session_type _session_type = Session_type::DEFAULT, string _session_name = "",
            Authentication _auth = Authentication()): url(_url) , method(_method),content(_content),session_type(_session_type),
                                    session_name(_session_name),auth(_auth),callback(_callback)
        {
            ignore_iterating_limit = false;
        }
        unordered_map<string,string> bundle;
        Task& put_string(string key, string value) {
            bundle[key] = value;
            return *this;
        }
        Task& ignore_iteration_limit(bool value) {
            ignore_iterating_limit = value;
            return *this;
        }
        Request_method get_method() {
            return method;
        }
        Request_content get_content() {
            return content;
        }
        Session_type get_session_type() {
            return session_type;
        }
        string get_session_name() {
            return session_name;
        }
        void set_session_name(string _session_name) {
            session_name = string();
            session_name = "" + _session_name;
        }
        string get_url() {
            return url;
        }
        spider_callback get_callback() {
            return callback;
        };
        bool ignore_iterating_limit;
    private:
        Request_method method;
        Request_content content;
        Session_type session_type;
        string session_name;
        string url;
        Authentication auth;
        spider_callback callback;
    };

}


#endif //CRAWLERPLUSPLUS_GIT_TASK_H
