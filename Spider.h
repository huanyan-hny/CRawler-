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
		vector<Request> next_reqs;
		vector<shared_ptr<Item>> items;
	};

	class Spider
	{
	public:
		void closed();
		string name;
		string allow_domains;
		virtual Request start_requests() = 0;
		virtual Parsed_Result parse(Response res) = 0;

	};
}