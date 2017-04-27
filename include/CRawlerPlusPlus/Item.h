#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <algorithm>
using namespace std;

namespace Crawler
{
	class File
	{
	public:
		string extension;
		vector<char> data;
	};

	class Item
	{
	public:
		string name;
		string type;
		unordered_map<string, int> fields_idx;
		unordered_map<string, File> data;
		vector<pair<string, string>> fields;
		string& operator [] (string key);
		string to_string();
		string to_csv();
		string to_csv_title();
        void add_file(string field, const vector<char>& bytes, string extension = "txt");
		void feed_file(string field, vector<char>& bytes, string extension = "txt");

	};

}
