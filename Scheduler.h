#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <thread>
#include <mutex>
#include <queue>
#include <atomic>
#include <climits>
#include "Network.h"
using namespace std;

namespace Crawler
{
	class Scheduler
	{
	public:
		Scheduler(int _max_layer = INT_MAX) 
		{
			max_layer = _max_layer;
			now_layer = 0;
		}
		queue<shared_ptr<Request>> current_q;
		queue<shared_ptr<Request>> next_q;

		mutex q_lk;

		void start_request(shared_ptr<Request> req)
		{
			q_lk.lock();
			current_q.push(req);
			q_lk.unlock();
		}
		void start_requests(vector<shared_ptr<Request>> reqs)
		{
			q_lk.lock();
			for (auto &entry : reqs)
				current_q.push(entry);
			q_lk.unlock();
		}

		void add_request(shared_ptr<Request> req)
		{
			q_lk.lock();
			next_q.push(req);
			q_lk.unlock();
		}
		void add_requests(vector<shared_ptr<Request>> reqs)
		{
			q_lk.lock();
			for (auto &entry : reqs)
				next_q.push(entry);
			q_lk.unlock();
		}
		shared_ptr<Request> get_request()
		{
			q_lk.lock();
			shared_ptr<Request> ret;
			if (current_q.empty())
				ret = nullptr;
			else
				ret = current_q.front();
			current_q.pop();
			q_lk.unlock();
			return ret;
		}
		int get_current_size()
		{
			q_lk.lock();
			auto ret = current_q.size();
			q_lk.unlock();
			return ret;
		}
		bool is_current_empty()
		{
			return current_q.empty();
		}
		void next_layer()
		{
			q_lk.lock();
			current_q = std::move(next_q);
/*
			current_q =  queue<shared_ptr<Request>>();
			while(!next_q.empty())
			{
				current_q.push(next_q.front());
				next_q.pop();
			}
			*/
			next_q = queue<shared_ptr<Request>>();
			now_layer++;
			q_lk.unlock();
			cout << "next layer " << now_layer << endl;
		}

		bool finished()
		{
//			cout << now_layer << endl;
//			cout << max_layer << endl;
			if (now_layer > max_layer) return true;
			return false;
		}
	private:
		int max_layer;
		int now_layer;
	};
}