//
// Created by elega on 4/26/17.
//

#include "include/CRawlerPlusPlus/Generic_Scheduler.h"
#include <assert.h>

using namespace Crawler;
void test_simple_scheduler()
{
    Generic_Scheduler s;
    vector<shared_ptr<Task>> start;
    start.push_back(make_shared<Task>(Task("start",nullptr)));
    s.start_requests(start);
    assert(s.get_size() == 1);
    assert(s.get_request()->get_url() == "start");

    cout<<"test_simple_scheduler: pass"<<endl;

}

void test_scheduler_iteration()
{
    Generic_Scheduler s;
    s.set_iterations(2);
    vector<shared_ptr<Task>> start;
    auto origin = make_shared<Task>(Task("start",nullptr));
    auto branch1 = make_shared<Task>(Task("b1",nullptr));
    auto branch2 = make_shared<Task>(Task("b2",nullptr));
    auto branch3 = make_shared<Task>(Task("b2",nullptr));

    auto it_3 = make_shared<Task>(Task("b4",nullptr));


    start.push_back(origin);
    s.start_requests(start);
    assert(s.get_size() == 1);
    assert(s.get_request()->get_url() == "start");

    thread th1([&](){
        s.add_request(branch1,origin);
    });

    thread th2([&](){
        s.add_request(branch2,origin);
    });


    thread th3([&](){
        s.add_request(branch3,origin);
    });

    th1.join();
    th2.join();
    th3.join();

    assert(s.get_size() ==2);
    s.add_request(branch1,it_3);
    assert(s.get_size() ==2);

    cout<<"test_scheduler_iteration: pass"<<endl;
}


int main()
{
    test_simple_scheduler();
    test_scheduler_iteration();
}