//
// Created by elega on 4/27/17.
//
#include "Item.h"
namespace Crawler {
    string &Item::operator[](string key) {
        if (fields_idx.count(key) == 0) {
            fields_idx[key] = fields.size();
            fields.push_back(pair<string, string>(key, ""));
            return fields[fields.size() - 1].second;
        } else
            return fields[fields_idx[key]].second;
    }

    string Item::to_string() {
        string s;
        std::for_each(fields.begin(), fields.end(), [&](pair<string, string> &item) {
            s = s + item.first + ":" + item.second + "\n";
        });
        return s;
    }

    string Item::to_csv() {
        string s;
        std::for_each(fields.begin(), fields.end(), [&](pair<string, string> &item) {
            string ss = item.second;
            for (int i = 0; i < ss.size(); ++i)
                if (ss[i] == ',') ss[i] = ' ';
            s = s + ss + ",";
        });
        s.resize(s.size() - 1);
        return s;
    }

    string Item::to_csv_title() {
        string s;
        std::for_each(fields.begin(), fields.end(), [&](pair<string, string> &item) {
            string ss = item.first;
            for (int i = 0; i < ss.size(); ++i)
                if (ss[i] == ',') ss[i] = ' ';
            s = s + ss + ",";
        });
        s.resize(s.size() - 1);
        return s;
    }


    void Item::add_file(string field, const vector<char> &bytes, string extension) {
        File f;
        f.extension = extension;
        f.data = bytes;
        data[field] = std::move(f);
    }

    void Item::feed_file(string field, vector<char> &bytes, string extension) {
        File f;
        f.extension = extension;
        f.data = std::move(bytes);
        data[field] = std::move(f);
    }
}