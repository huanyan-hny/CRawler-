#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <assert.h>
using namespace std;

namespace Crawler
{
	class Item
	{
	public:
		unordered_map<string, string> data;
		string& operator [] (string key)
		{
			if (data.count(key) != 0)
				return data[key];
			assert(false && "item key not found");
		}
	protected:
		void define_field(string field)
		{
			if (data.count(field) == 0)
				data[field] = "";
		}
	};
}