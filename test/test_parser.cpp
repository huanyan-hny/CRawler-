#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <assert.h>
#include "include/CRawlerPlusPlus/Parser.h"
using namespace Crawler;

void test_find_tag(HTML h) {
    vector<HTML> divs = h.find_all("div","");
    assert(divs.size()==752);
    cout << "Test case:Find tag(Pass)" << endl;
}

void test_find_selfclosing_tag(HTML h) {
    vector<HTML> links = h.find_all("link","");
    assert(links.size()==14);
    cout << "Test case:Find self-closing tag (Pass)" << endl;
}

void test_find_tag_with_selector(HTML h) {
    vector<HTML> urls = h.find_all("a","ref_=tt_rec_tti");
    vector<HTML> names = h.find_all("img","height=\"113\", width=\"76\"");
    assert(urls.size()==names.size()*2);
    cout << "Test case:Find tag with selector (Pass)" << endl;
}

void test_find_xpath(HTML h) {
    vector<HTML> res = h.find_by_xpath("div/span/a/");
    assert(res.size()==203);
    cout << "Test case:Find tag with selector (Pass)" << endl;
}

void test_get_attribute(HTML h) {
    string class_attribute = h.getAttribute("class");
    string id_attribute = h.getAttribute("id");
    assert(class_attribute=="Myclass");
    assert(id_attribute=="Myid");
    cout << "Test case:Get attribute (Pass)" << endl;
}

void test_get_innerhtml(HTML h) {
    string innerHTML = h.getInnerHTML();
    assert(innerHTML=="myInnerHTML");
    cout << "Test case:Get innerHTML (Pass)" << endl;
}

int main(){
    ifstream in("src.html");
    stringstream ss;
    ss << in.rdbuf();
    string content = ss.str();

    HTML h = HTML(content);
    test_find_tag(h);
    test_find_selfclosing_tag(h);
    test_find_tag_with_selector(h);
    test_find_xpath(h);

    string html = "<div class=\"Myclass\", id=\"Myid\">myInnerHTML</div>";

    HTML t = HTML(html);

    test_get_attribute(t);
    test_get_innerhtml(t);

    cout << "All test case passed" << endl;
    return 0;
}