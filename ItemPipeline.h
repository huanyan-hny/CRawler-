#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include "Item.h"
using namespace std;

namespace Crawler
{
	class ItemPipeline
	{
	public:
		void open_spider(shared_ptr<Spider> spider);
		void close_spider(shared_ptr<Spider> spider);
		void handle_items(vector<shared_ptr<Item>>& items);
	protected:
		virtual Item process_item(shared_ptr<Item> item, shared_ptr<Spider> spider) = 0;

	};
}