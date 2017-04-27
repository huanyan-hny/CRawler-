#pragma once
//http://stackoverflow.com/questions/15752659/thread-pooling-in-c11
#include "Generic_Scheduler.h"
#include "Downloader.h"
#include "Spider.h"
#include "ItemPipeline.h"
#include "IMDBSpider.h"
#include "IMDBItemPipeline.h"
#include "Scheduler_Concepts.h"
#include <iostream>
#include <memory>
#include <thread>
#include <future>
using namespace std;

//http://stackoverflow.com/questions/26577317/implementation-of-then-for-stdfuture-results-in-compilation-error

namespace Crawler
{
	template <Scheduler T>
	class Engine
	{
	public:
		Engine(shared_ptr<Spider> _spider, shared_ptr<ItemPipeline> _item_pipeline ) {
			max_threads = 1;
			spider = _spider;
			item_pipeline = _item_pipeline;
			scheduler = make_unique<Generic_Scheduler>(INT_MAX);
			scheduler->start_requests(spider->initial_tasks_wrapper());
		}
		void set_max_threads(int t)
		{
			if (t>0)
				max_threads = t;
		}
		void set_iterations(int l)
		{
			scheduler->set_iterations(l);
		}
		void start()
		{
			item_pipeline->open_spider(spider);
			while (!scheduler->is_empty() || active_threads.load() > 0)
			{
				if (active_threads.load()> max_threads) {
					continue;
				}

				shared_ptr<Task> task = scheduler->get_request();
				if (task == nullptr) {
					continue;
				}
				atomic_fetch_add(&active_threads, 1);
				store_future( std::async(std::launch::async, [&](shared_ptr<Task> task)
				{
					Task t = *task;
//					Request req = Request("get",task->get_url(),task->get_content());
					shared_ptr<Response> res = cd.get(*task);



					// should be
//                    shared_ptr<Response> res = Downloader::Curl_Downloader::get(*task);
					std::async(std::launch::async, [&](shared_ptr<Task> old_tsk, shared_ptr<Response> res) {
						auto result = spider->parse_wrapper(old_tsk,res);
						vector<shared_ptr<Item>> items = result.items;
						vector<shared_ptr<Task>> reqs = result.next_reqs;
						scheduler->add_requests(reqs,old_tsk);
						item_pipeline->handle_items(items);
						atomic_fetch_add(&active_threads, -1);
					},task,res);
				},task));


			}
			item_pipeline->close_spider(spider);

		}

		void start_async(std::function<void(void)> callback)
		{
			thread t([&](){
				start();
				callback();
			});
			t.detach();

		}
	private:
		shared_ptr<Spider> spider;
		unique_ptr<Generic_Scheduler> scheduler;
		shared_ptr<ItemPipeline> item_pipeline;
		Downloader::Curl_Downloader cd;
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