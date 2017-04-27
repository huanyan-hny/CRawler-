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
#include "Task.h"
#include "Network.h"
using namespace std;

namespace Crawler
{
    class Generic_Scheduler
    {
    public:
        Generic_Scheduler(int _max_layer = INT_MAX);
        queue<shared_ptr<Task>> requests_pool;
        unordered_map<string,int> hash_table;

        mutex q_lk;

        void start_request(shared_ptr<Task> req);
        void start_requests(vector<shared_ptr<Task>> reqs);

        void add_request(shared_ptr<Task> new_req, shared_ptr<Task> old_req);
        void add_requests(vector<shared_ptr<Task>> new_reqs, shared_ptr<Task> old_req );
        shared_ptr<Task> get_request();
        int get_size();
        bool is_empty();
        bool finishing();
        void set_iterations(int l);

    private:
        int max_layer;
        int now_layer;
    };
}

