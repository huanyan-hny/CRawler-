#pragma once
//http://stackoverflow.com/questions/15752659/thread-pooling-in-c11
#include "Generic_Scheduler.h"
#include "Downloader.h"
#include "Spider.h"
#include "ItemPipeline.h"
#include <iostream>
#include <memory>
#include <thread>
#include <future>
using namespace std;

//http://stackoverflow.com/questions/26577317/implementation-of-then-for-stdfuture-results-in-compilation-error

namespace Crawler
{
    template <class T>
	class Engine
	{	
	public:
		Engine(int _max_threads, int _max_layers) 
		{
			spider = make_unique<Spider>();
			item_pipeline = make_unique<ItemPipeline>();
			scheduler = make_unique<Generic_Scheduler>(_max_layers);
			scheduler->start_requests(spider->start_requests());
			max_threads = _max_threads;
		}
		void start()
		{
			
			while (!scheduler->is_empty() || active_threads.load() > 0)
			{
				if (active_threads.load()> max_threads) {
					continue;
				}

				shared_ptr<Request> req = scheduler->get_request();
				if (req == nullptr) {
					continue;
				}
				atomic_fetch_add(&active_threads, 1);
				store_future( std::async(std::launch::async, [&](shared_ptr<Request> req)
				{

                    shared_ptr<Response> res = Downloader::sync_download(req);
					std::async(std::launch::async, [&](shared_ptr<Request> old_req, shared_ptr<Response> res) {
						auto result = spider->parse(res);
						vector<shared_ptr<Item>> items = result.items;
						vector<shared_ptr<Request>> reqs = result.next_reqs;
                        scheduler->add_requests(reqs,old_req);
						item_pipeline->handle_items(items);
						atomic_fetch_add(&active_threads, -1);
					},req,res);
				},req));


			}
		}

		private:
		unique_ptr<Spider> spider;
		unique_ptr<Generic_Scheduler> scheduler;
		unique_ptr<ItemPipeline> item_pipeline;

		std::atomic_int active_threads{0};
		mutex future_lk;
		vector<future<void>> pending_futures;

		int max_threads;
		
		inline void store_future(future<void>&& f)
		{
			future_lk.lock();
			if (pending_futures.size() > 1000)
				pending_futures.clear();
			pending_futures.push_back(std::move(f));
			future_lk.unlock();

		}

	};

}