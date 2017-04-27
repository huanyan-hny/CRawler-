//
// Created by elega on 4/26/17.
//

#ifndef CRAWLERPLUSPLUS_GIT_SCHEDULER_CONCEPTS_H
#define CRAWLERPLUSPLUS_GIT_SCHEDULER_CONCEPTS_H

#include <vector>
#include <memory>

using namespace std;
namespace Crawler {

class Task;

//template<typename T>
//concept bool Scheduler = requires (T s) {
//    {new T(0)} -> T*;
//    {s.set_iterations(0)} -> void;
//    {s.start_request(shared_ptr<Task>())} -> void;
//    {s.start_requests(vector<shared_ptr<Task>>())} -> void;
//    {s.add_request(shared_ptr<Task>(),shared_ptr<Task>())} -> void;
//    {s.add_requests(vector<shared_ptr<Task>>(), shared_ptr<Task>())} -> void;
//    {s.get_request()} -> shared_ptr<Task>;
//    {s.get_size()} -> int;
//    {s.is_empty()} -> bool;
//    {s.finishing()} -> bool;
//};




}
#endif //CRAWLERPLUSPLUS_GIT_SCHEDULER_CONCEPTS_H
