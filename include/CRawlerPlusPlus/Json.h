#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <string.h>
#include <algorithm>
using namespace std;

enum JsonValue {some,null};

class Json {
public:
    Json();
    
    Json(unordered_map<string, Json> m);
    
    Json(string s);
    
    Json(int i);
    
    Json(double d);
    
    Json(bool b);
    
    Json(JsonValue j);
    
    Json(vector<Json> l);
    
    static Json parse(string jsonString){
        unordered_map<string, Json> m;
        string content = jsonString.substr(1,jsonString.length()-2);
        int quoteCount = 0;
        int leftListCount = 0;
        int leftJsonCount = 0;
        int pivot = 0;
        bool inkey = true;
        bool invalue = false;
        string key;
        for(int i=0;i<content.length();i++) {
            if (inkey) {
                if (content[i]=='"') {
                    quoteCount++;
                    if (quoteCount==2) {
                        key = content.substr(pivot+1,i-pivot-1);
                        inkey = false;
                        quoteCount = 0;
                    }
                }
            } else if (invalue) {
                if (i == content.length()-1 && content[i]!='"') {
                    string value = content.substr(pivot,i-pivot+1);
                    m[key] = checkValue(value);
                } else if (content[i]=='"' && !leftJsonCount && !leftListCount) {
                    quoteCount++;
                    if (quoteCount==2) {
                        string s = content.substr(pivot+1,i-pivot-1);
                        m[key] = Json(s);
                        invalue = false;
                        quoteCount = 0;
                    }
                } else if (content[i]=='[' && !leftJsonCount) {
                    leftListCount++;
                    if (leftListCount==1)
                        pivot = i;
                } else if (content[i]=='{' && !leftListCount) {
                    leftJsonCount++;
                    if (leftJsonCount==1)
                        pivot = i;
                } else if (content[i]==']' && !leftJsonCount) {
                    leftListCount--;
                } else if (content[i]=='}' && !leftListCount) {
                    leftJsonCount--;
                } else if (content[i]==',' && !leftJsonCount && !leftListCount) {
                    string value = content.substr(pivot,i-pivot);
                    m[key] = checkValue(value);
                    pivot = i+1;
                    invalue = false;
                    inkey = true;
                }
            } else {
                if (content[i]==':') {
                    pivot = i+1;
                    invalue = true;
                } else if (content[i]==',') {
                    pivot = i+1;
                    inkey = true;
                }
            }
        }
        
        
        return Json(m);
    }
    
    Json operator[](string key);
    Json operator[](int index);
    size_t size();
    string toString();
    
    string getString();
    
    int getInt();
    
    double getDouble();
    
    bool getBool();
    
    vector<Json> getList();
    
    JsonValue getNull();
    
    string getType();
    
    vector<Json> find_by_key(string key);
    
    template<typename T>
    T get() {}
    
private:
    string type;
    unordered_map<string, Json> mapValue;
    string stringvalue;
    int intValue;
    double doubleValue;
    bool boolValue;
    vector<Json> listValue;
    JsonValue jValue;
    
    static bool isInt(string s) {
        for (char c:s)
            if (!isdigit(c))
                return false;
        return true;
    }
    
    static bool isDouble(string s) {
        try {
            stod(s);
        }
        catch(...) {
            return false;
        }
        return true;
    }
    
    static bool isBool(string s) {
        return s=="true" || s=="false";
    }
    
    static bool isNull(string s) {
        return s=="null";
    }
    
    static Json checkValue(string value) {
        if (isInt(value))
            return Json(stoi(value));
        else if (isDouble(value))
            return Json(stod(value));
        else if (isBool(value))
            return Json(value=="true"?true:false);
        else if (isNull(value))
            return Json(null);
        else if (value.front()=='{' && value.back()=='}')
            return parse(value);
        else
            return checkListValue(value);
    }
    
    static vector<Json> checkListValue(string value) {
        vector<Json> res;
        string content = value.substr(1,value.length()-2);
        int quoteCount = 0;
        int pivot = 0;
        int leftListCount = 0;
        int leftJsonCount = 0;
        for (int i=0;i<content.length();i++) {
            if (i == content.length()-1 && content[i]!='"') {
                string value = content.substr(pivot,i-pivot+1);
                res.push_back(checkValue(value));
            } else if (content[i]=='[' && !leftJsonCount) {
                leftListCount++;
                if (leftListCount==1)
                    pivot = i;
            } else if (content[i]=='{' && !leftListCount) {
                leftJsonCount++;
                if (leftJsonCount==1)
                    pivot = i;
            } else if (content[i]==']' && !leftJsonCount) {
                leftListCount--;
            } else if (content[i]=='}' && !leftListCount) {
                leftJsonCount--;
            } else if (content[i]=='"' && !leftListCount && !leftJsonCount) {
                quoteCount++;
                if (quoteCount==2) {
                    string strvalue = content.substr(pivot+1,i-pivot-1);
                    res.push_back(Json(strvalue));
                    quoteCount = 0;
                    i++;
                    pivot = i+1;
                }
            } else if (content[i]==',' && !leftListCount && !leftJsonCount) {
                string value = content.substr(pivot,i-pivot);
                res.push_back(checkValue(value));
                pivot = i+1;
            }
        }
        return res;
    }
};