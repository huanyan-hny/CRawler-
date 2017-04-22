#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <iostream>
#include <fstream>
#include "Item.h"
#include <boost/filesystem.hpp>
using namespace std;

namespace Crawler
{

	class ItemPipeline
	{
	public:
		void open_spider(shared_ptr<Spider> spider) {}
		void close_spider(shared_ptr<Spider> spider) {}
		virtual void items_feeded(shared_ptr<Spider> spider) {}
		void handle_items(vector<shared_ptr<Item>>& items )
		{
			for (auto item : items)
			{
				process_item(item);
			}

			static int cnt = 0;
			cout << "handling " << cnt << endl<<flush;
			cnt++;

		}
		virtual void process_item(shared_ptr<Item> item)
		{
			simple_write(item);
		}

	protected:
		
		void simple_write(shared_ptr<Item> item)
		{
			boost::filesystem::path tmp = boost::filesystem::unique_path();
			string filename =  tmp.string() + ".txt";
			cout << "writing on " << filename << endl;
			ofstream ofs;
			ofs.open(filename, std::ofstream::out);
			ofs << item->to_string();
			ofs.close();
//			item->to_string();
		}
		void simple_file_write(shared_ptr<Item> item)
		{
			for (pair<string, File> file_item : item->data)
			{
				boost::filesystem::path tmp = boost::filesystem::unique_path();
				string filename = "./output/" + tmp.string() + file_item.second.extension;
				ofstream ofs;
				ofs.open(filename, std::ofstream::out);
				ofs.write(file_item.second.data.data(), file_item.second.data.size());
				ofs.close();

			}
		}
		

	};
}
