
#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Item.h"
#include "Network.h"
using namespace std;

namespace Crawler
{
    struct Parsed_Result
    {
        vector<shared_ptr<Request>> next_reqs;
        vector<shared_ptr<Item>> items;
    };

    class Spider
    {
    public:
        virtual vector<shared_ptr<Request>> start_requests() = 0;
        virtual Parsed_Result parse(shared_ptr<Request> req, shared_ptr<Response> res) = 0;
        string get_domains(){return domains;};
        void set_domains(string d) {domains = d;};
        string get_name(){return name;};
        void set_name(string n) {name = n;};
        virtual void opened(){}
        virtual void closed(){}
    protected:
        string name;
        string domains;
    };
}