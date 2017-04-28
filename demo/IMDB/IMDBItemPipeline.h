#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <iostream>
#include <fstream>
#include "include/CRawlerPlusPlus/Item.h"
#include "include/CRawlerPlusPlus/ItemPipeline.h"
#include <boost/filesystem.hpp>
using namespace std;

namespace Crawler
{

	class IMDBItemPipeline: public ItemPipeline
	{
	public:
        ofstream outputer;
		void open_spider(shared_ptr<Spider> spider) {
            string filename =  "./output/movies.csv";
            outputer.open(filename, std::ofstream::out);
        }
		void close_spider(shared_ptr<Spider> spider) {
            outputer.close();
        }
		void process_item(shared_ptr<Item> item)
        {
            if (item->type == "movie") {
                static bool first = true;
                if (first) {
                    outputer << item->to_csv_title() << endl;
                    first = false;
                }
                outputer << item->to_csv() << endl;
            }
            else if (item->type == "poster")
                simple_file_write("./output",item->data["poster"],item->name);
		}

	};
}
