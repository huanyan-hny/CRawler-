//
// Created by Yanlin Duan on 4/20/17.
//

#ifndef CRAWLERPLUSPLUS_GIT_UTIL_H
#define CRAWLERPLUSPLUS_GIT_UTIL_H

#endif //CRAWLERPLUSPLUS_GIT_UTIL_H

#include <string>
#include <algorithm>
#include "Network.h"

#define FWD(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

using namespace std;

namespace Crawler_Util{
    string to_lower(string s){
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }

    template <typename C>
    void set_option(C& c) {
    }

    template <typename C, typename T>
    void set_option(C& c, T&& t) {
        c.SetOption(FWD(t));
    }

    template <typename C, typename T, typename... Ts>
    void set_option(C& c, T&& t, Ts&&... ts) {
        set_option(c, FWD(t));
        set_option(c, FWD(ts)...);
    }

}