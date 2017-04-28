#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <algorithm>
using namespace std;

namespace Crawler
{
	///
	/// The file class, one instance represents a file.\n
	/// Bytes are stored in a char vector.
	///
	class File
	{
	public:
		string extension;
		vector<char> data;
	};


	///
	/// The Item class\n
	/// A item is produced by Spider.\n
	/// A item can carry two kinds of data:
	/// simple string data and file.\n
	/// also could store both at the same time
	///
	class Item
	{
	public:
		string name; ///< the name of the item, usually each item instance has a distinct name
		string type; ///< the type of the item, e.x., "link", "poster", "table", used for routing different items
		unordered_map<string, int> fields_idx;	///< hash map to store the order of fields
		unordered_map<string, File> data;	///< hash map to store files
		vector<pair<string, string>> fields;	///< vector to store fields

		///
		/// Find the value of a field of the item\n
		/// if the field is not defined, it will be created automatically\n
		/// fields has orders, see fields_idx
		/// \param key the field of the item
		/// \return the value of that field
		///
		string& operator [] (string key);
		string to_string();	///< a simple helper to convert a item to string
		string to_csv();	///< a simple heler to convert a item to csv, comma will be replaced by white space
		string to_csv_title();	///<a simple heler to convert the name of field of a item to csv, comma will be replaced by white space

		///
		/// Add a file to the item\n
		/// the file will be copy and stored
		/// \param field the field you want to store it
		/// \param bytes the file in vector<char>
		/// \param extension the extension of that file
		///
		void add_file(string field, const vector<char>& bytes, string extension = "txt");

		///
		/// Feed a file to the item\n
		/// like add_file, but the filed will be moved and the content of param "byte" will be cleared\n
		/// no copy happens with the help of std::move
        ///
		void feed_file(string field, vector<char>& bytes, string extension = "txt");

	};

}
