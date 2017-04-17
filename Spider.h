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
		void closed();
		string name;
		string allow_domains;
		virtual vector<shared_ptr<Request>> start_requests()
		{
			vector<shared_ptr<Request>> v;
			v.push_back(make_shared<Request>(Request()));
			return v;
		}
		virtual Parsed_Result parse(shared_ptr<Response> res)
		{
			Parsed_Result ret;
			ret.next_reqs.push_back(make_shared<Request>(Request())) ;
			ret.next_reqs.push_back(make_shared<Request>(Request()));

			Item i;
			i["foo"] = "foo";
			i["bar"] = "bar";
			i["foobar"] = "barfoo";
	
			ret.items.push_back(make_shared<Item>(i));
			return ret;

			}

	};
}