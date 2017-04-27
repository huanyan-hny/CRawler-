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

template<typename T>
concept bool Scheduler = requires (T s) {
    {s.set_iterations(0)} -> void
    {s.start_request(shared_ptr<Task>())} -> void;
};




}
#endif //CRAWLERPLUSPLUS_GIT_SCHEDULER_CONCEPTS_H
