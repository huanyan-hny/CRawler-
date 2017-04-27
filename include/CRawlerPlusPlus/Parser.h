#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

namespace Crawler {
    struct ParserObj{

        string s;

        ParserObj(string _s);

        static bool isnum(string s);

        ParserObj getAttribute(string attribute);

        ParserObj getInnerHTML();

        vector<ParserObj> extract(string path, bool isLast);

        vector<ParserObj> find_by_xpath(string xpath);

        vector<ParserObj> find_all(string tag, string selectors);

        ParserObj find_one(string tag, string selectors);

        string to_string();

    };

    namespace Parser {

        bool isnum(string s);

        string getAttribute(string html, string attribute);

        string getInnerHTML(string html);


        vector<string> extract(string src, string path, bool isLast);

        vector<string> find_by_xpath(string src, string xpath);


        vector<string> find_all(string src, string tag, string selectors);

        string find_one(string src, string tag, string selectors);

    }
}
