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

    typedef unordered_map<string,string> Form;
    typedef function<void(shared_ptr<Task>,shared_ptr<Response>,function<void(Task&)>,function<void(Item&)> )>
            spider_callback;
    class Task {
    public:
        /*
        template <typename... Tail>
        Task(string _url, spider_callback _callback, Tail... tail): url(_url), callback(_callback)
        {
            method = Request_method::GET;
            content = Request_content::STRING;
            session_type = Session_type::DEFAULT;
            session_name = "";
            auth = Authentication("","");
            form = {};
            Crawler_Util::set_option(*this, tail...);
        }
         */
        Task()
        {}
        Task(string _url,
             spider_callback _callback,
             Request_method _method = Request_method::GET, Request_content _content = Request_content::STRING,
             Session_type _session_type = Session_type::DEFAULT, string _session_name = "",
            Authentication _auth = Authentication(), Form _form = {}): url(_url) , method(_method),content(_content),session_type(_session_type),
                                    session_name(_session_name),auth(_auth),callback(_callback), form(_form)
        {
            ignore_iterating_limit = false;
//            cout<<"new "<< (long)this << " " <<url <<endl;
        }


        ~Task()
        {
//            cout<<"release " << (long)this <<" " << url <<endl;
        }
        unordered_map<string,string> bundle;

        void set_option(spider_callback&& _callback){
            callback = _callback;
        }

        void set_option(const spider_callback& _callback){
            callback = _callback;
        }

        void set_option(Request_method _method){
            method = _method;
        }

        void set_option(Request_content _content){
            content = _content;
        }

        void set_option(Session_type _session_type) {
            session_type = _session_type;
        }

        void set_option(string _session_name){
            session_name = _session_name;
        }

        void set_option(Authentication&& _auth){
            auth = _auth;
        }

        void set_option(const Authentication& _auth){
            auth = _auth;
        }

        void set_option(Form&& _form){
            form = _form;
        }

        void set_option(const Form& _form){
            form = _form;
        }

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
//            cout<<"url "<<url<<endl;
            return url;
        }
        spider_callback get_callback() {
            return callback;
        };
        auto get_authentication() {
            return auth;
        }
        auto get_form() {
            return form;
        }
        bool ignore_iterating_limit;

        string url;
    private:
        Request_method method;
        Request_content content;
        Session_type session_type;
        string session_name;

        Authentication auth;
        spider_callback callback;
        Form form;
    };

}


#endif //CRAWLERPLUSPLUS_GIT_TASK_H
