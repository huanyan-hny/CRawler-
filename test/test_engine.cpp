//
// Created by elega on 4/26/17.
//

#include "include/CRawlerPlusPlus/Engine.h"
#include "include/CRawlerPlusPlus/Spider.h"
#include "include/CRawlerPlusPlus/ItemPipeline.h"
#include <assert.h>
#include <iostream>
#include <algorithm>
#include <functional>
using namespace std;

using namespace Crawler;
    class TestSpider : public Spider {
    public:
        void initial_tasks(function<void(Task &)> add_task) {
            add_task(Task("www.stroustrup.com/Bjarne.jpg", callback(&TestSpider::parse_image),Request_method::GET,
                          Request_content::FILE).
                    put_string("type", "link"));
        }

        void parse_image(shared_ptr<Task> task, shared_ptr<Response> res,
                         function<void(Task &)> add_task, function<void(Item &)> produce_item) {
            Item i;
            i.type = "img";
            i.name = "Bjarne";
            i.feed_file("img", res->content, "jpg");
            produce_item(i);
        }

    };


    class TestItemPipeline : public ItemPipeline {
    public:
        void process_item(shared_ptr<Item> item) {
            simple_file_write("./output",item->data["img"], item->name);
        }

    };

    void test_engine() {
        shared_ptr < TestSpider > spider = make_shared<TestSpider>();
        shared_ptr < TestItemPipeline > pipeline = make_shared<TestItemPipeline>();
        Crawler::Engine<Crawler::Generic_Scheduler> e(spider, pipeline);
        e.set_max_threads(1);
        e.set_iterations(1);
        e.start();

        ifstream ifs("./output/Bjarne.jpg",ios::in);
        assert(ifs.is_open());
        cout<<"test_engine: pass"<<endl;
    }

    int main()
    {
        test_engine();
    }
