#include "Json.h"

Json::Json(){};
    
Json::Json(unordered_map<string, Json> m) {
    type = "Json";
    mapValue = m;
}
    
Json::Json(string s) {
    type = "string";
    stringvalue = s;
}
    
Json::Json(int i) {
    type = "int";
    intValue = i;
}
    
Json::Json(double d) {
    type = "double";
    doubleValue = d;
}
    
Json::Json(bool b) {
    type = "bool";
    boolValue = b;
}
    
Json::Json(JsonValue j) {
    type = "null";
    jValue = null;
}
    
Json::Json(vector<Json> l) {
    type = "list";
    listValue = l;
}

Json Json::operator[](string key) {
    return mapValue[key];
}

Json Json::operator[](int index) {
    return listValue[index];
}
    
size_t Json::size() {
    if (type=="Json")
        return mapValue.size();
    else if (type=="list")
        return listValue.size();
    return 0;
}
    
string Json::toString() {
    if (type=="string") {
        return '"' + stringvalue + '"';
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
    
string Json::getString() {
    return stringvalue;
}
    
int Json::getInt() {
    return intValue;
}
    
double Json::getDouble() {
    return doubleValue;
}
    
bool Json::getBool() {
    return boolValue;
}
    
vector<Json> Json::getList() {
    return listValue;
}
    
JsonValue Json::getNull() {
    return null;
}
    
string Json::getType() {
    return type;
}
    
vector<Json> Json::find_by_key(string key) {
    vector<Json> res;
    vector<Json> more;
    if (type=="Json") {
        for (auto element:mapValue) {
            string k = element.first;
            Json j = element.second;
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

template<> string Json::get<string>() {
    return stringvalue;
}

template<> int Json::get<int>() {
    return intValue;
}

template<> double Json::get<double>() {
    return doubleValue;
}

template<> bool Json::get<bool>() {
    return boolValue;
}

template<> vector<Json> Json::get<vector<Json>>() {
    return listValue;
}

template<> JsonValue Json::get<JsonValue>() {
    return jValue;
}