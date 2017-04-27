//
// Created by elega on 4/27/17.
//
#include "Spider.h"
namespace Crawler
{

    vector<shared_ptr<Task>> Spider::initial_tasks_wrapper()
    {
        vector<shared_ptr<Task>> ret;
        initial_tasks([&](Task& req){
            ret.push_back(make_shared<Task>(req));
        } );
        return ret;
    }
    Parsed_Result Spider::parse_wrapper(shared_ptr<Task> task, shared_ptr<Response> res)
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

    string Spider::get_domains(){return domains;};
    void Spider::set_domains(string d) {domains = d;};
    string Spider::get_name(){return name;};
    void Spider::set_name(string n) {name = n;};

}