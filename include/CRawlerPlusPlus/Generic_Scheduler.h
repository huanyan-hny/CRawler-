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
    ///
    /// The scheduler class
    /// This is a generic, thread-safe scheduler,
    /// implemented by a std::queue with std::lock.\n
    /// Duplicates could be removed automatically
    ///
    class Generic_Scheduler
    {
    public:
        Generic_Scheduler(int _max_layer = INT_MAX); ///< the construct with one param to set the maximum iterations
        queue<shared_ptr<Task>> requests_pool;  ///< the queue to store tasks
        unordered_map<string,int> hash_table;   ///< the hash_table to remove duplicates

        mutex q_lk;

        void start_request(shared_ptr<Task> req);   ///< add one initial task
        void start_requests(vector<shared_ptr<Task>> reqs); ///< add initial tasks

        ///
        /// Add a new following tasks.\n
        /// Duplicates will be removed.\n
        /// Task exceeds the maximum iteration will not be added.\n
        /// \param new_req the task produced by the old one
        /// \param old_req the old task that the Spider took to produce the new task
        ///
        void add_request(shared_ptr<Task> new_req, shared_ptr<Task> old_req);

        ///
        /// Add new following tasks, see add _request.\n
        /// \param new_req the task produced by the old one
        /// \param old_req the old task that the Spider took to produce the new task
        ///
        void add_requests(vector<shared_ptr<Task>> new_reqs, shared_ptr<Task> old_req );

        shared_ptr<Task> get_request(); ///< get a request; if the queue is empty, return null
        int get_size(); ///< return the the size of pending tasks
        bool is_empty();    ///< return if the queue is empty
        bool finishing();   ///< return if the queue is empty
        void set_iterations(int l); ///< set the maximum iterations

    private:
        int max_layer;
        int now_layer;
    };
}

