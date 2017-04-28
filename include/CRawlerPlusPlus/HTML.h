#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

namespace Crawler {
    ///
    /// The HTML class, an encapsulation of string. \n
    /// Provides parsing for HTML source code
    class HTML {
    public:
        string getAttribute(string attribute); ///< Get the specified attribute of the HTML element
        string getInnerHTML (); ///< Get the enclosed innerHTML of the HTML element
        vector<HTML> extract(string path, bool isLast); ///<Extract the content of the given tag path tag_name[attributes], e.g. div[class="header"]
        vector<HTML> find_by_xpath(string xpath); ///< Extract the content of the given xpath, e.g. /div/span[class="avatar"]/img
        vector<HTML> find_all(string tag, string selectors); ///< Find all content of the given tag (e.g div) with attribute selectors (e.g class="header")
        HTML(){}
        HTML(string s){src=s;}
    private:
        string src;
        static bool isnum(string s) {
            for (char c:s)
                if (!isdigit(c))
                    return false;
            return true;
        }
    };
    
}