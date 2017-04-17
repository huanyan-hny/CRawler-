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
			static int cnt = 0;
			cout << "downloading " << cnt << endl;
			cnt++;
			std::this_thread::sleep_for(std::chrono::milliseconds(750 + rand() % 500 ));
			auto ret = make_shared<Response>(Response());
			return ret;

		}
		//async 
		//std::async ?
		//std::future ?
		//std::thread ?
	};
}