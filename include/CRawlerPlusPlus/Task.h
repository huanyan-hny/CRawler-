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

    enum class Auth_type {
        NONE,
        BASIC,
        OAUTH2,
    };

    class Item;
    class Task;

    typedef unordered_map<string,string> Form;
    typedef function<void(shared_ptr<Task>,shared_ptr<Response>,function<void(Task&)>,function<void(Item&)> )>
            spider_callback;
    class Task {
    public:

        template <typename... Tail>
        Task(string _url, spider_callback _callback, Tail... tail): url(_url), callback(_callback)
        {
            method = Request_method::GET;
            content = Request_content::HTML;
            session_type = Session_type::DEFAULT;
            session_name = "";
            auth = Authentication("","");
            form = {};
            Crawler_Util::set_option(*this, tail...);
        }

        Task() {}
        Task(string _url,
             spider_callback _callback,
             Request_method _method = Request_method::GET, Request_content _content = Request_content::HTML,
             Session_type _session_type = Session_type::DEFAULT, string _session_name = "",
             Auth_type _auth_type = Auth_type::NONE,
             Authentication _auth = Authentication(), Form _form = {}, Header _header={});

        unordered_map<string,string> bundle;

        void set_option(spider_callback&& _callback);
        void set_option(const spider_callback& _callback);
        void set_option(Request_method _method);
        void set_option(Auth_type _auth_type) ;
        void set_option(Request_content _content);
        void set_option(Session_type _session_type);
        void set_option(Header _header);
        void set_option(string _session_name);
        void set_option(Authentication&& _auth);
        void set_option(const Authentication& _auth);
        void set_option(Form&& _form);
        void set_option(const Form& _form);
        Task& put_string(string key, string value);
        Task& ignore_iteration_limit(bool value);
        Request_method get_method();
        Request_content get_content();
        Session_type get_session_type();
        string get_session_name();
        void set_session_name(string _session_name);
        string get_url();
        spider_callback get_callback();
        Authentication get_authentication();
        Header get_header();
        Auth_type get_auth_type();
        Form get_form();
        bool ignore_iterating_limit;

        string url;
    private:
        Request_method method;
        Request_content content;
        Session_type session_type;
        string session_name;

        Authentication auth;
        Auth_type auth_type;
        spider_callback callback;
        Form form;
        Header header;
    };

}


#endif //CRAWLERPLUSPLUS_GIT_TASK_H
