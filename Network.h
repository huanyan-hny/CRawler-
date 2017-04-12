#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <assert.h>
using namespace std;

namespace Crawler
{
	typedef string request_payload;
	typedef string response_payload;

	class Request
	{
	public:
		request_payload asio_request;
	};

	class Response
	{
	public:
		response_payload aiso_response;
	};
}