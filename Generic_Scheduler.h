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
    class Generic_Scheduler
    {
    public:
        Generic_Scheduler(int _max_layer = INT_MAX)
        {
            cout<<"init scheduler"<<endl;
            max_layer = _max_layer;
            now_layer = 0;
        }
        queue<shared_ptr<Request>> requests_pool;
        unordered_map<string,int> hash_table;

        mutex q_lk;

        void start_request(shared_ptr<Request> req)
        {
            q_lk.lock();
            while(!requests_pool.empty()) requests_pool.pop();
            hash_table.clear();
            requests_pool.push(req);
            hash_table[req->get_url()] = 1;
            q_lk.unlock();
        }
        void start_requests(vector<shared_ptr<Request>> reqs)
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

        void add_request(shared_ptr<Request> new_req, shared_ptr<Request> old_req)
        {
            q_lk.lock();
            auto new_url = new_req->get_url();
            auto old_url = old_req->get_url();
            if (hash_table[old_url] < max_layer && (hash_table.count(new_url)==0 || true) ) {
                hash_table[new_url] = hash_table[old_url]+1;
                if (hash_table[new_url] > now_layer)
                {
                    cout<<"now layer "<< hash_table[new_url] <<endl;
                    now_layer = hash_table[new_url];
                }
                requests_pool.push(new_req);
            }
            q_lk.unlock();
        }
        void add_requests(vector<shared_ptr<Request>> new_reqs, shared_ptr<Request> old_req )
        {
            q_lk.lock();
            auto old_url = old_req->get_url();
            for (auto &new_req : new_reqs) {
                auto new_url = new_req->get_url();

                if (hash_table[old_url] < max_layer && (hash_table.count(new_url)==0 || true) ) {
                    hash_table[new_url] = hash_table[old_url]+1;
                    if (hash_table[new_url] > now_layer)
                    {
                        cout<<"now layer "<< hash_table[new_url] <<endl;
                        now_layer = hash_table[new_url];
                    }
                    requests_pool.push(new_req);
                }
            }
            q_lk.unlock();
        }
        shared_ptr<Request> get_request()
        {
            q_lk.lock();
            shared_ptr<Request> ret;
            if (requests_pool.empty())
                ret = nullptr;
            else {
                ret = requests_pool.front();
                requests_pool.pop();

            }
            q_lk.unlock();
            return ret;
        }
        int get_size()
        {
            q_lk.lock();
            auto ret = requests_pool.size();
            q_lk.unlock();
            return ret;
        }
        bool is_empty()
        {
            return requests_pool.empty();
        }

        bool finishing()
        {
            if (now_layer > max_layer && requests_pool.empty()) return true;
            return false;
        }
    private:
        int max_layer;
        int now_layer;
    };
}