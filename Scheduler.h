#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include "Network.h"
using namespace std;

namespace Crawler
{
	class Scheduler
	{
	public:
		void add_request(Request req);
		void add_requests(vector<Request> req);

		Request get_request();
		int get_size();
		bool is_empty();
	};
}