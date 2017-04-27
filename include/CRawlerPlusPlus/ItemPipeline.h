#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <iostream>
#include <fstream>
#include "Item.h"
#include <boost/filesystem.hpp>
#include <mutex>
using namespace std;

namespace Crawler
{

	class Spider;
	class ItemPipeline
	{
	public:
		void handle_items(vector<shared_ptr<Item>>& items );
		virtual void process_item(shared_ptr<Item> item) = 0;
        virtual void items_feeded(shared_ptr<Spider> spider) {}
        virtual void open_spider(shared_ptr<Spider> spider) {};
        virtual void close_spider(shared_ptr<Spider> spider) {};

	protected:
		void simple_write(shared_ptr<Item> item);
		void simple_file_write(shared_ptr<Item> item);
		void simple_file_write(const File& file,string filename);
		string string_to_filename(string input);
    private:
        std::mutex lck;
		const std::string forbiddenChars = "\\/:?\"<>| \n\r\t";


	};
}


