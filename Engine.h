#pragma once
//http://stackoverflow.com/questions/15752659/thread-pooling-in-c11
#include "Scheduler.h"
#include "Downloader.h"
#include "Spider.h"
#include "ItemPipeline.h"
#include <iostream>
#include <memory>
using namespace std;

namespace Crawler
{
	class Engine
	{	
	public:
		Engine() 
		{
			scheduler.add_request(spider->start_requests());
		}

		void test()
		{
			while (!scheduler.is_empty())
			{
				Request req = scheduler.get_request();
				Response res = Downloader::sync_download(req);
				auto result = spider->parse(res);
				vector<shared_ptr<Item>> items = result.items;
				vector<Request> reqs = result.next_reqs;
				scheduler.add_requests(reqs);
				item_pipeline->handle_items(items);
			}
		}
	private:
		unique_ptr<Spider> spider;
		Scheduler scheduler;
		unique_ptr<ItemPipeline> item_pipeline;

	};
}