//
// Created by elega on 4/27/17.
//

#include "Task.h"

namespace Crawler {

    Task::Task(string _url,
               spider_callback _callback,
               Request_method _method, Request_content _content ,
               Session_type _session_type, string _session_name ,
               Auth_type _auth_type ,
               Authentication _auth, Form _form , Header _header): url(_url) , method(_method),content(_content),session_type(_session_type),
                                                                   session_name(_session_name),auth(_auth),callback(_callback), form(_form), header(_header)
    {
        ignore_iterating_limit = false;
    }
    void Task::set_option(spider_callback&& _callback){
        callback = _callback;
    }

    void Task::set_option(const spider_callback& _callback){
        callback = _callback;
    }

    void Task::set_option(Request_method _method){
        method = _method;
    }

    void Task::set_option(Auth_type _auth_type) {
        auth_type = _auth_type;
    }

    void Task::set_option(Request_content _content){
        content = _content;
    }

    void Task::set_option(Session_type _session_type) {
        session_type = _session_type;
    }

    void Task::set_option(Header _header){
        header = _header;
    }
    void Task::set_option(string _session_name){
        session_name = _session_name;
    }

    void Task::set_option(Authentication&& _auth){
        auth = _auth;
    }

    void Task::set_option(const Authentication& _auth){
        auth = _auth;
    }

    void Task::set_option(Form&& _form){
        form = _form;
    }

    void Task::set_option(const Form& _form){
        form = _form;
    }

    Task& Task::put_string(string key, string value) {
        bundle[key] = value;
        return *this;
    }
    Task& Task::ignore_iteration_limit(bool value) {
        ignore_iterating_limit = value;
        return *this;
    }
    Request_method Task::get_method() {
        return method;
    }
    Request_content Task::get_content() {
        return content;
    }
    Session_type Task::get_session_type() {
        return session_type;
    }
    string Task::get_session_name() {
        return session_name;
    }
    void Task::set_session_name(string _session_name) {
        session_name = string();
        session_name = "" + _session_name;
    }
    string Task::get_url() {
//            cout<<"url "<<url<<endl;
        return url;
    }
    spider_callback Task::get_callback() {
        return callback;
    };
    Authentication Task::get_authentication() {
        return auth;
    }
    Header Task::get_header() {
        return header;
    }

    Auth_type Task::get_auth_type() {
        return auth_type;
    }
    Form Task::get_form() {
        return form;
    }


}
