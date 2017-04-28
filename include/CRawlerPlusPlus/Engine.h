#pragma once
#include "Generic_Scheduler.h"
#include "Downloader.h"
#include "Spider.h"
#include "ItemPipeline.h"
#include "Scheduler_Concepts.h"
#include <iostream>
#include <memory>
#include <thread>
#include <future>
using namespace std;

namespace Crawler
{
    ///
    /// The engine class\n
    /// The routine of an engine is to get tasks from scheduler,\n
    /// use downloader to download pages (or files),\n
    /// then feed then to a user-defined parser to analyze and produce items,\n
    /// at last give data to item pipeline to store them.\n
    /// The engine is a multithreaded one.
    ///
//	template <Scheduler T>
    template <typename T>
	class Engine
	{
	public:
        ///
        /// \param _spider a shared pointer of any dereived class from Spider,
        /// \param _item_pipeline a shared pointer of any dereived class from ItemPipeline
        /// The constructor
        ///
		Engine(shared_ptr<Spider> _spider, shared_ptr<ItemPipeline> _item_pipeline );
		void set_max_threads(int t); ///< set the number of threads working at the same time
		void set_iterations(int l); ///< set the number of crawling iterations
		void start(); ///< start the engine synchronously, will not return until finished
		void start_async(std::function<void(void)> callback); ///< start the engine asynchronously, after finishing, the callback will be invoked
	private:
		shared_ptr<Spider> spider; ///< the user-defined spider instance
		unique_ptr<Generic_Scheduler> scheduler;///< the pre-defined scheduler
		shared_ptr<ItemPipeline> item_pipeline; ///< the user-defined item pipeline
		Downloader::Curl_Downloader cd;///< the downloader implemented by curl
		std::atomic_int active_threads{0}; ///< the counter of active threads
		mutex future_lk;
		vector<future<void>> pending_futures; ///< used to collect std::feature, otherwise it will be blocked
		int max_threads;
        inline void store_future(future<void>&& f);
	};

    template <class T>
    Engine<T>::Engine(shared_ptr<Spider> _spider, shared_ptr<ItemPipeline> _item_pipeline ) {
        max_threads = 1;
        spider = _spider;
        item_pipeline = _item_pipeline;
        scheduler = make_unique<Generic_Scheduler>(INT_MAX);
        scheduler->start_requests(spider->initial_tasks_wrapper());
    }

    template <class T>
    void Engine<T>::set_max_threads(int t)
    {
        if (t>0)
            max_threads = t;
    }

    template <class T>
    void Engine<T>::set_iterations(int l)
    {
        scheduler->set_iterations(l);
    }

    template <class T>
    void Engine<T>::start()
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
                shared_ptr<Response> res = cd.get(*task);
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

    template <class T>
    void Engine<T>::start_async(std::function<void(void)> callback)
    {
        thread t([&](){
            start();
            callback();
        });
        t.detach();
    }

    template <class T>
    inline void Engine<T>::store_future(future<void>&& f)
    {
        future_lk.lock();
        if (pending_futures.size() > 1000)
            pending_futures.clear();
        pending_futures.push_back(std::move(f));
        future_lk.unlock();

    }


}