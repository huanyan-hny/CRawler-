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
    struct Parsed_Result
    {
        vector<shared_ptr<Task>> next_reqs;
        vector<shared_ptr<Item>> items;
    };

    class Spider
    {
    public:
        vector<shared_ptr<Task>> initial_tasks_wrapper();
        virtual Parsed_Result parse_wrapper(shared_ptr<Task> task, shared_ptr<Response> res);
        virtual void initial_tasks(function<void(Task&)> add_task) = 0;

        string get_domains();
        void set_domains(string d);
        string get_name();
        void set_name(string n);

    protected:
        string name;
        string domains;

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
