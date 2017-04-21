#pragma once
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include "Network.h"
using namespace std;

namespace Crawler
{
	class Downloader
	{
	public:
		static shared_ptr<Response> sync_download(shared_ptr<Request> req)
		{
			return req->get();

		}
		//async 
		//std::async ?
		//std::future ?
		//std::thread ?
	};
}