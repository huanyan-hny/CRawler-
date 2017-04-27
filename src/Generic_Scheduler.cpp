//
// Created by elega on 4/27/17.
//

#include "Generic_Scheduler.h"


namespace Crawler
{
    Generic_Scheduler::Generic_Scheduler(int _max_layer)
    {
        cout<<"init scheduler"<<endl;
        max_layer = _max_layer;
        now_layer = 0;
    }
    void Generic_Scheduler::start_request(shared_ptr<Task> req)
    {
        q_lk.lock();
        while(!requests_pool.empty()) requests_pool.pop();
        hash_table.clear();
        requests_pool.push(req);
        hash_table[req->get_url()] = 1;
        q_lk.unlock();
    }
    void Generic_Scheduler::start_requests(vector<shared_ptr<Task>> reqs)
    {

        q_lk.lock();
        while(!requests_pool.empty()) requests_pool.pop();
        hash_table.clear();
        for (auto &req : reqs) {
            requests_pool.push(req);
            hash_table[req->get_url()] = 1;
        }
        q_lk.unlock();
    }

    void Generic_Scheduler::add_request(shared_ptr<Task> new_req, shared_ptr<Task> old_req)
    {
        q_lk.lock();
        auto new_url = new_req->get_url();
        auto old_url = old_req->get_url();
        if (hash_table.count(new_url)==0) {
            if (hash_table[old_url] < max_layer  && !new_req->ignore_iterating_limit) {
                hash_table[new_url] = hash_table[old_url] + 1;
                requests_pool.push(new_req);
            }
            else if (hash_table[old_url] <= max_layer && new_req->ignore_iterating_limit ) {
                hash_table[new_url] = hash_table[old_url];
                requests_pool.push(new_req);
            }
        }
        q_lk.unlock();
    }
    void Generic_Scheduler::add_requests(vector<shared_ptr<Task>> new_reqs, shared_ptr<Task> old_req )
    {
        q_lk.lock();
        auto old_url = old_req->get_url();
        for (auto &new_req : new_reqs) {
            auto new_url = new_req->get_url();

            if (hash_table.count(new_url)==0) {
                if (hash_table[old_url] < max_layer && !new_req->ignore_iterating_limit) {
                    hash_table[new_url] = hash_table[old_url] + 1;
                    requests_pool.push(new_req);

                } else if (hash_table[old_url] <= max_layer && new_req->ignore_iterating_limit) {
                    hash_table[new_url] = hash_table[old_url];
                    requests_pool.push(new_req);
                }
            }
        }
        q_lk.unlock();
    }
    shared_ptr<Task> Generic_Scheduler::get_request()
    {
        q_lk.lock();
        shared_ptr<Task> ret;
        if (requests_pool.empty())
            ret = nullptr;
        else {
            ret = requests_pool.front();
            requests_pool.pop();

            string new_url = ret->get_url();
            if (hash_table[new_url] > now_layer) {
                cout << "iteration: " << hash_table[new_url] << endl;
                now_layer = hash_table[new_url];
            }
        }
        q_lk.unlock();
        return ret;
    }
    int Generic_Scheduler::get_size()
    {
        q_lk.lock();
        auto ret = requests_pool.size();
        q_lk.unlock();
        return ret;
    }
    bool Generic_Scheduler::is_empty()
    {
        return requests_pool.empty();
    }

    bool Generic_Scheduler::finishing()
    {
        if (now_layer > max_layer && requests_pool.empty()) return true;
        return false;
    }

    void Generic_Scheduler::set_iterations(int l)
    {
        if (l > 0 )
            max_layer = l;
    }

}