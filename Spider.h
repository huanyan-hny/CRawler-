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
			v.push_back(make_shared<Request>(Request("get","www.imdb.com/title/tt0796366/")));
			return v;
		}
		virtual Parsed_Result parse(shared_ptr<Response> res)
		{
			Parsed_Result ret;
			ret.next_reqs.push_back(make_shared<Request>(Request("get","www.imdb.com/title/tt0796366/"))) ;
			ret.next_reqs.push_back(make_shared<Request>(Request("get","www.imdb.com/title/tt0796366/")));

			Item i;
			i["response"] = res->asio_response;
			i["bar"] = "bar";
			i["foobar"] = "barfoo";
	
			ret.items.push_back(make_shared<Item>(i));
			return ret;

			}

	};
}