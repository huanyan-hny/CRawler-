#ifndef CRAWLERPLUSPLUS_GIT_SPIDER_H
#define CRAWLERPLUSPLUS_GIT_SPIDER_H
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "Item.h"
#include "Network.h"
#include "Task.h"
using namespace std;

namespace Crawler
{
    ///
    /// The Parsed_Result class, acting as a std::pair
    ///
    struct Parsed_Result
    {
        vector<shared_ptr<Task>> next_reqs;
        vector<shared_ptr<Item>> items;
    };


    ///
    /// The Spider class\n
    /// Be used to parse http response and produce items\n
    /// The user should inherit this class and override the initial_tasks() method\n
    /// The user also need to write the function to parse the http response\n
    /// e.g. void parse(shared_ptr<Task> task, shared_ptr<Response> res, function<void(Task&)> add_task, function<void(Item&)> produce_item)\n
    /// The handler takes four params, the first one is the task, the second one is the response,
    /// the third one is a callback, tasks are added by this function,
    /// the forth one is also a callback, items are produced by this function.
    /// Both work like yield return.
    ///
    class Spider
    {
    public:
        vector<shared_ptr<Task>> initial_tasks_wrapper(); ///< wrapped the initial_tasks function
        ///
        /// wrapped user defined parse handler to parse http response, called by engine\n
        /// user defined handler will be called in this method with four parameters, see spider_callback\n
        /// the user defined handler must be registered in handler by Spider::callback() function\n
        /// \param task the request which generates the response
        /// \param res the response
        /// \return the parsed result, including task for next crawling and items to process
        ///
        Parsed_Result parse_wrapper(shared_ptr<Task> task, shared_ptr<Response> res);

        /// the function to add initial tasks, tasks are added by add_task callback function\n
        /// the user must override the function in his derived class.
        /// \param add_task the callback handler, used to add a task, like yield return
        virtual void initial_tasks(function<void(Task&)> add_task) = 0;

        string get_domains();        ///< get the domain of the spider
        void set_domains(string d); ///< set the domain of the spider
        string get_name();  ///< get the name of the spider
        void set_name(string n);    ///set the name of the spider

    protected:
        string name;
        string domains;

        ///
        /// Register a callback, used when creating a new task\n
        /// The callback will be invoked in parse_wrapper function when the response is ready\n
        /// i.e. Task("google",callback(&MySpider::foo)\n
        /// \return spider_callback, which could be used as a param in the constructor in Task()
        ///
        template<class T>
        spider_callback callback(void (T::* callback)
                (shared_ptr<Task>,shared_ptr<Response>,function<void(Task&)>,function<void(Item&)>))
        {
            return std::bind(callback, (T*)this, std::placeholders::_1,  std::placeholders::_2,
                             std::placeholders::_3,std::placeholders::_4);
        }
    };
}


#endif
