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

    /// Task object -- a substitution for request. \n
    /// This abstraction contains more information and gives user more flexibility in terms of what to store
    /// and how to store.
    class Task {
    public:

        /// This function is the truly optional constructor for Task.
        /// \tparam Tail Variadic template for optional parameters
        /// \param _url  url string
        /// \param _callback  callback function. Set to nullptr if no callback.
        /// \param tail other optional parameters/
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
        ///
        /// \param _url URL
        /// \param _callback Callback function ptr. Set to nullptr if no callback
        /// \param _method GET/POST
        /// \param _content FILE/JSON/HTML
        /// \param _session_type A Session_type enum class
        /// \param _session_name string that will be matched in Downloader's SessionMap
        /// \param _auth_type NONE/BASIC/OAUTH2
        /// \param _auth detailed authentication information. If none, just put Authentication().
        /// \param _form form for payload. If none, just do Form {}.
        /// \param _header Any custom header (for example, user-agent, or referer). Some websites will check this.
        Task(string _url,
             spider_callback _callback,
             Request_method _method = Request_method::GET, Request_content _content = Request_content::HTML,
             Session_type _session_type = Session_type::DEFAULT, string _session_name = "",
             Auth_type _auth_type = Auth_type::NONE,
             Authentication _auth = Authentication(), Form _form = {}, Header _header={});

        unordered_map<string,string> bundle;

        /// Set callback (r-value)
        /// \param _callback
        void set_option(spider_callback&& _callback);
        /// Set callback (l-value)
        /// \param _callback
        void set_option(const spider_callback& _callback);
        /// Set request method;
        /// \param _method
        void set_option(Request_method _method);
        /// Set authentication type
        /// \param _auth_type
        void set_option(Auth_type _auth_type) ;
        /// Set request content type (FILE/JSON/HTML)
        /// \param _content
        void set_option(Request_content _content);
        /// Set session option
        /// \param _session_type
        void set_option(Session_type _session_type);
        /// Set custom header
        /// \param _header
        void set_option(Header _header);
        /// Set session name (if applicable)
        /// \param _session_name
        void set_option(string _session_name);
        /// Set authentication detail info (r-value)
        /// \param _auth An authentication object (with username and password)
        void set_option(Authentication&& _auth);
        /// Set authentication detail info (l-value)
        /// \param _auth An authentication object (with username and password)
        void set_option(const Authentication& _auth);
        /// Set form info (r-value)
        /// \param _form
        void set_option(Form&& _form);
        /// Set form info (l-value)
        /// \param _form
        void set_option(const Form& _form);
        /// Put key-value pairs in bundle. Can be used in a chain.
        /// Such as t.put_string("a","b").put_string("c","d");
        /// \param key
        /// \param value
        /// \return Reference to this task after change.
        Task& put_string(string key, string value);
        /// Whether this task will ignore iteration limit.
        /// \param value
        /// \return Reference to this task after change.
        Task& ignore_iteration_limit(bool value);
        /// Get GET/PUT method
        /// \return
        Request_method get_method();
        /// Get which type of request content (FILE/HTML/JSON)
        /// \return
        Request_content get_content();
        /// Get session type
        /// \return
        Session_type get_session_type();
        /// Get sesison name
        /// \return
        string get_session_name();
        /// Set session name
        /// \param _session_name
        void set_session_name(string _session_name);
        /// Get url from Task
        /// \return
        string get_url();
        /// Get callback linked with the task.
        /// \return A function object/pointer with placeholder. See spider_callback type for detail.
        spider_callback get_callback();
        /// Get authentication object (a copy)
        /// \return A copy of the authentication (including both username and password).
        Authentication get_authentication();
        /// Get header
        /// \return
        Header get_header();
        /// Get authentication type of the task
        /// \return
        Auth_type get_auth_type();
        /// Get the form (payload) of this task as a copy.
        /// \return A copy of the payload form.
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
