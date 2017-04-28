#include "Json.h"
namespace Crawler{
    JSON::JSON(){};
        
    JSON::JSON(unordered_map<string, JSON> m) {
        type = "JSON";
        mapValue = m;
    }
        
    JSON::JSON(string s) {
        type = "string";
        stringvalue = s;
    }
        
    JSON::JSON(int i) {
        type = "int";
        intValue = i;
    }
        
    JSON::JSON(double d) {
        type = "double";
        doubleValue = d;
    }
        
    JSON::JSON(bool b) {
        type = "bool";
        boolValue = b;
    }
        
    JSON::JSON(JSONValue j) {
        type = "null";
        jValue = null;
    }
        
    JSON::JSON(vector<JSON> l) {
        type = "list";
        listValue = l;
    }

    JSON JSON::operator[](string key) {
        return mapValue[key];
    }

    JSON JSON::operator[](int index) {
        return listValue[index];
    }
        
    size_t JSON::size() {
        if (type=="JSON")
            return mapValue.size();
        else if (type=="list")
            return listValue.size();
        return 0;
    }
        
    string JSON::toString() {
        if (type=="string") {
            return "\"" + stringvalue + "\"";
        }
        else if (type=="int")
            return to_string(intValue);
        else if (type=="double")
            return to_string(doubleValue);
        else if (type=="bool")
            return to_string(boolValue);
        else if (type=="null")
            return "null";
        else if (type=="list") {
            string res = "[";
            for (int i=0;i<listValue.size();i++) {
                res += listValue[i].toString();
                if (i<listValue.size()-1)
                    res += ",";
            }
            res += "]";
            return res;
        } else {
            string res = "{";
            size_t counter = mapValue.size();
            for (auto it = mapValue.begin();it!=mapValue.end();it++) {
                res += "\"" + it->first + "\":";
                res += it->second.toString();
                counter--;
                if (counter>0)
                    res += ",";
            }
            res += "}";
            return res;
        }
    }
        
    string JSON::getString() {
        return stringvalue;
    }
        
    int JSON::getInt() {
        return intValue;
    }
        
    double JSON::getDouble() {
        return doubleValue;
    }
        
    bool JSON::getBool() {
        return boolValue;
    }
        
    vector<JSON> JSON::getList() {
        return listValue;
    }
        
    JSONValue JSON::getNull() {
        return null;
    }
        
    string JSON::getType() {
        return type;
    }

    unordered_map<string, JSON> JSON::getJSON() {
        return mapValue;
    }
        
    vector<JSON> JSON::find_by_key(string key) {
        vector<JSON> res;
        vector<JSON> more;
        if (type=="JSON") {
            for (auto element:mapValue) {
                string k = element.first;
                JSON j = element.second;
                if (k==key) {
                    res.push_back(j);
                }
                more = j.find_by_key(key);
                res.insert(res.end(),more.begin(),more.end());
            }
        } else if (type=="list") {
            for (auto element:listValue) {
                more = element.find_by_key(key);
                res.insert(res.end(),more.begin(),more.end());
            }
        }
        return res;
    }

    template<> string JSON::get<string>() {
        return stringvalue;
    }

    template<> int JSON::get<int>() {
        return intValue;
    }

    template<> double JSON::get<double>() {
        return doubleValue;
    }

    template<> bool JSON::get<bool>() {
        return boolValue;
    }

    template<> vector<JSON> JSON::get<vector<JSON>>() {
        return listValue;
    }

    template<> JSONValue JSON::get<JSONValue>() {
        return jValue;
    }
}