#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <assert.h>
#include "include/CRawlerPlusPlus/Json.h"
using namespace Crawler;

void test_find_by_key(JSON j) {
    JSON k = j.find_by_key("testKey")[0];
    assert(k.getType()=="string" && k.getString()=="testValue");
    cout << "Test case:Find by key (Pass)" << endl;
}
void test_operator_overload_json(JSON j) {
    JSON k = j["testJSON"]["a"];
    assert(k.getType()=="string" && k.getString()=="b");
    cout << "Test case:Operator overload JSON (Pass)" << endl;
}

void test_operator_overload_list(JSON j) {
    JSON k = j["testList"][0];
    assert(k.getType()=="int" && k.getInt()==1);
    cout << "Test case:Operator overload List (Pass)" << endl;
}

void test_get_element(JSON j) {
    assert(j["testInt"].getType()=="int" && j["testInt"].getInt()==42);
    assert(j["testDouble"].getType()=="double" && j["testDouble"].getDouble()==4.2);
    assert(j["testString"].getType()=="string" && j["testString"].getString()=="string");
    assert(j["testBool"].getType()=="bool" && j["testBool"].getBool()==true);
    cout << "Test case:Get type and value (Pass)" << endl;
}

int main(int argc, const char * argv[]) {
    JSON j = JSON::parse("{\"testKey\":\"testValue\",\"testInt\":42,\"testDouble\":4.2,\"testString\":\"string\",\"testBool\":true,\"testJSON\":{\"a\":\"b\"},\"testList\":[1,2,3]}");
    test_find_by_key(j);
    test_operator_overload_json(j);
    test_operator_overload_list(j);
    test_get_element(j);
    cout << "All test case passed" << endl;
    return 0;
}