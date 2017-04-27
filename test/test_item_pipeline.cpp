//
// Created by elega on 4/26/17.
//

#include "../ItemPipeline.h"
#include <assert.h>
#include <iostream>
#include <algorithm>
#include <functional>
using namespace std;
using namespace Crawler;


void test_item_pipeline()
{
    class my_ip : public ItemPipeline {
        void process_item(shared_ptr<Item> item) {
            static int count = 0;
            if (count ==0 ) {
                assert(item->to_csv() == "a,b");
                count++;
            } else
                assert(item->to_csv() == "aa,bb");
        }
    };

    my_ip ip;
    Item i1;
    i1["foo"] = "a";
    i1["bar"] = "b";

    Item i2;
    i2["foo"] = "aa";
    i2["bar"] = "bb";

    vector<shared_ptr<Item>> v;
    v.push_back(make_shared<Item>(i1));
    v.push_back(make_shared<Item>(i2));

    ip.handle_items(v);

    cout<<"test_item_pipeline: pass"<<endl;
}


void test_item_write_str()
{
    class my_ip : public ItemPipeline {
        void process_item(shared_ptr<Item> item) {
            simple_write(item);
        }
    };

    my_ip ip;
    Item i1;
    i1["foo"] = "a";
    i1["bar"] = "b";

    Item i2;
    i2["foo"] = "aa";
    i2["bar"] = "bb";

    vector<shared_ptr<Item>> v;
    v.push_back(make_shared<Item>(i1));
    v.push_back(make_shared<Item>(i2));

    ip.handle_items(v);

    cout<<"test_item_write_str: pass"<<endl;
}



void test_item_file_write_str()
{
    class my_ip : public ItemPipeline {
        void process_item(shared_ptr<Item> item) {
            simple_file_write(item->data["aaa"],"test");
        }
    };

    my_ip ip;
    Item i;
    vector<char> vv;
    vv.push_back('a');
    vv.push_back('b');
    vv.push_back('c');

    i.feed_file("aaa",vv);
    vector<shared_ptr<Item>> v;
    v.push_back(make_shared<Item>(i));

    ip.handle_items(v);

    ifstream ifs("./output/test.txt",ios::in);
    assert(ifs.is_open());
    char buf[10];
    ifs.read(buf,3);
    assert((unsigned char)buf[0] ==(unsigned char)'a');
    assert((unsigned char)buf[1] ==(unsigned char)'b');
    assert((unsigned char)buf[2] ==(unsigned char)'c');
    ifs.close();
    cout<<"test_item_file_write_str: pass"<<endl;
}

int main()
{
    test_item_pipeline();
    test_item_write_str();
    test_item_file_write_str();
}