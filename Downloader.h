#pragma once
#include <string>
#include <vector>
#include <functional>
#include "Network.h"
using namespace std;

namespace Crawler
{
	class Downloader
	{
	public:
		static Response sync_download(Request req);
		//async 
		//std::async ?
		//std::future ?
		//std::thread ?
	};
}