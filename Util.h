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
        c.set_option(FWD(t));
    }

    template <typename C, typename T, typename... Ts>
    void set_option(C& c, T&& t, Ts&&... ts) {
        set_option(c, FWD(t));
        set_option(c, FWD(ts)...);
    }

    std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
    }

    std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        split(s, delim, elems);
        return elems;
    }

    string cpr_header_to_string(cpr::Header h) {
        string s;

        for( auto i=h.begin(); i != h.end(); ++i )
        {
            s += i->first + ": " + i->second +"\r\n";
        }
        s += "\r\n";
        return s;
    }

    static size_t write_to_vec(void *contents, size_t size, size_t nmemb, void *userp)
    {
        size_t realsize = size * nmemb;
        vector<char> *vb = (vector<char> *)userp;
        for (int offset = 0; offset < realsize ; offset++) {
            vb->push_back(*((char *)(contents) + offset));
        }
        return realsize;
    }


}