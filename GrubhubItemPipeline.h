#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <iostream>
#include <fstream>
#include "Item.h"
#include "ItemPipeline.h"
#include <boost/filesystem.hpp>
using namespace std;

namespace Crawler
{

    class GrubhubItemPipeline: public ItemPipeline
    {
    public:
        ofstream outputer;
        void open_spider(shared_ptr<Spider> spider) {
            string filename =  "./grubhub/restaurant.csv";
            outputer.open(filename, std::ofstream::out);
        }
        void close_spider(shared_ptr<Spider> spider) {
            outputer.close();
        }
        void process_item(shared_ptr<Item> item)
        {
            if (item->type == "menu")
                outputer<<item->to_csv()<<endl;
        }

    };
}
