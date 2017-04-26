
#pragma once
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
        vector<shared_ptr<Task>> initial_tasks_wrapper()
        {
            vector<shared_ptr<Task>> ret;
            initial_tasks([&](Task& req){
                ret.push_back(make_shared<Task>(req));
            } );
            return ret;
        }
        virtual Parsed_Result parse_wrapper(shared_ptr<Task> task, shared_ptr<Response> res)
        {
            Parsed_Result ret;
            task->get_callback()(task,res,[&](Task& t){
                switch (t.get_session_type()) {
                    case Session_type::AUTO:
//NEED TO IMPLEMENT
                        t.set_session_name(".......");
                        break;
                    case Session_type::DEFAULT:
                        t.set_session_name("default_session");
                        break;
                    case Session_type::LAST:
                        t.set_session_name(task->get_session_name());
                        break;
                    case Session_type::NEW:
                        break;
                    case Session_type::SPECIFIED:
                        break;
                }
                ret.next_reqs.push_back(make_shared<Task>(t));

            },[&](Item& item) {
                ret.items.push_back(make_shared<Item>(item));
            });
            return ret;
        }

        virtual void initial_tasks(function<void(Task&)> add_task) = 0;

        string get_domains(){return domains;};
        void set_domains(string d) {domains = d;};
        string get_name(){return name;};
        void set_name(string n) {name = n;};
        virtual void opened(){}
        virtual void closed(){}

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