//
// Created by elega on 4/26/17.
//

#include "../Item.h"
#include <assert.h>
#include <iostream>
#include <algorithm>
#include <functional>
using namespace std;
using namespace Crawler;
void test_item_field()
{
    Item i;
    i["foo"] = "a";
    i["foo"] += "a";
    assert(i["foo"] == "aa");
    i["bar"] = "b";
    assert(i.to_csv() == "aa,b");
    assert(i.to_csv_title() == "foo,bar");
    cout<<"test_item_field: pass"<<endl;
}

void test_item_file()
{
    Item i;
    vector<char> v;
    v.push_back('a');
    v.push_back('b');
    v.push_back('c');

    i.feed_file("aaa",v);
    assert(i.data["aaa"].data[0] == 'a');
    assert(i.data["aaa"].data[1] == 'b');
    assert(i.data["aaa"].data[2] == 'c');
    cout<<"test_item_file: pass"<<endl;

}

int main()
{
    test_item_field();
    test_item_file();
    return 0;
}