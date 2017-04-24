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
            if (item->type == "movie")
                outputer<<item->to_csv()<<endl;
            else if (item->type == "poster")
                simple_file_write(item->data["poster"],item->name);
		}

	};
}
