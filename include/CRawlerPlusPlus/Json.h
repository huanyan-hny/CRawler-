#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

namespace Crawler {
    enum JSONValue {some,null}; ///< A simple enum used to support null value

    ///
    /// The JSON class\n
    /// Provides parsing for JSON string
    ///

    class JSON {
    public:
        JSON();
        
        JSON(unordered_map<string, JSON> m);
        
        JSON(string s);
        
        JSON(int i);
        
        JSON(double d);
        
        JSON(bool b);
        
        JSON(JSONValue j);
        
        JSON(vector<JSON> l);
        
        ///
        /// The static parse method to convert a JSON string to a JSON object
        ///

        static JSON parse(string jsonString){
            unordered_map<string, JSON> m;
            string content = jsonString.substr(1,jsonString.length()-2);
            int quoteCount = 0;
            int leftListCount = 0;
            int leftJSONCount = 0;
            int pivot = 0;
            bool inkey = true;
            bool invalue = false;
            bool instring = false;
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
                    } else if (content[i]=='"' && !leftJSONCount && !leftListCount) {
                        instring = true;
                        quoteCount++;
                        if (quoteCount==2) {
                            string s = content.substr(pivot+1,i-pivot-1);
                            m[key] = JSON(s);
                            invalue = false;
                            instring = false;
                            quoteCount = 0;
                        }
                    } else if (content[i]=='[' && !leftJSONCount) {
                        leftListCount++;
                        if (leftListCount==1)
                            pivot = i;
                    } else if (content[i]=='{' && !leftListCount) {
                        leftJSONCount++;
                        if (leftJSONCount==1)
                            pivot = i;
                    } else if (content[i]==']' && !leftJSONCount) {
                        leftListCount--;
                    } else if (content[i]=='}' && !leftListCount) {
                        leftJSONCount--;
                    } else if (content[i]==',' && !leftJSONCount && !leftListCount && !instring) {
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
            
            
            return JSON(m);
        }
        
        JSON operator[](string key); ///< Overloaded operator for mapValue access
        JSON operator[](int index); ///< Overloaded operator for listValue access

        size_t size(); 

        string toString(); ///< Pretty Printing method
        
        string getString(); ///< Get the underlying string value (if underlying type is string)
        
        int getInt(); ///< Get the underlying int value (if underlying type is int)
        
        double getDouble(); ///< Get the underlying double value (if underlying type is double)
        
        bool getBool(); ///< Get the underlying Bool value (if underlying type is Bool)
        
        vector<JSON> getList(); ///< Get the underlying list value (if underlying type is list)
        
        JSONValue getNull(); ///< Get the underlying null value (if underlying type is null)
        
        string getType(); ///< Get the underlying type

        unordered_map<string, JSON> getJSON(); ///< Get the underlying map value (if underlying type is map)
        

        ///
        /// The searching method, returns all possible values of the given key
        ///

        vector<JSON> find_by_key(string key); 

        template<typename T>
        T get() {}
        
    private:
        string type;
        unordered_map<string, JSON> mapValue;
        string stringvalue;
        int intValue;
        double doubleValue;
        bool boolValue;
        vector<JSON> listValue;
        JSONValue jValue;
        
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
        
        static JSON checkValue(string value) {
            if (isInt(value))
                return JSON(stoi(value));
            else if (isDouble(value))
                return JSON(stod(value));
            else if (isBool(value))
                return JSON(value=="true"?true:false);
            else if (isNull(value))
                return JSON(null);
            else if (value.front()=='{' && value.back()=='}')
                return parse(value);
            else
                return checkListValue(value);
        }
        
        static vector<JSON> checkListValue(string value) {
            vector<JSON> res;
            string content = value.substr(1,value.length()-2);
            int quoteCount = 0;
            int pivot = 0;
            int leftListCount = 0;
            int leftJSONCount = 0;
            bool instring = false;
            for (int i=0;i<content.length();i++) {
                if (i == content.length()-1 && content[i]!='"') {
                    string value = content.substr(pivot,i-pivot+1);
                    res.push_back(checkValue(value));
                } else if (content[i]=='[' && !leftJSONCount) {
                    leftListCount++;
                    if (leftListCount==1)
                        pivot = i;
                } else if (content[i]=='{' && !leftListCount) {
                    leftJSONCount++;
                    if (leftJSONCount==1)
                        pivot = i;
                } else if (content[i]==']' && !leftJSONCount) {
                    leftListCount--;
                } else if (content[i]=='}' && !leftListCount) {
                    leftJSONCount--;
                } else if (content[i]=='"' && !leftListCount && !leftJSONCount) {
                    instring = true;
                    quoteCount++;
                    if (quoteCount==2) {
                        string strvalue = content.substr(pivot+1,i-pivot-1);
                        res.push_back(JSON(strvalue));
                        instring = false;
                        quoteCount = 0;
                        i++;
                        pivot = i+1;
                    }
                } else if (content[i]==',' && !leftListCount && !leftJSONCount && !instring) {
                    string value = content.substr(pivot,i-pivot);
                    res.push_back(checkValue(value));
                    pivot = i+1;
                }
            }
            return res;
        }
    };
};