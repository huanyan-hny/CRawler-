#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

namespace Crawler {
    struct ParserObj{
        string s;
        ParserObj(string _s): s(_s) {};
        static bool isnum(string s) {
            for (char c:s)
                if (!isdigit(c))
                    return false;
            return true;
        }

        ParserObj getAttribute(string attribute) {
            string res = "";
            attribute = attribute + "=\"";
            auto pos = s.find(attribute);
            if (pos != string::npos) {
                pos = pos + attribute.length();
                for (auto i = pos; i < s.length(); i++) {
                    if (s[i] == '"') {
                        res = s.substr(pos, i - pos);
                        break;
                    }
                }
            }

            return ParserObj(res);
        }

        ParserObj getInnerHTML() {
            int start, end;
            start = 0;
            end = (int) s.size() - 1;

            while (s[start] != '>')
                start++;

            while (s[end] != '<')
                end--;

            if (start >= end)
                return ParserObj("");
            else
                return ParserObj(s.substr(start + 1, end - start - 1));

        }


        vector<ParserObj> extract(string path, bool isLast) {

            vector<ParserObj> res;
            vector<string> selectors;
            string tag, selector;
            int index;
            bool isvoid;

            isvoid = false;
            tag = path;
            index = -1;

            auto left_bracket = path.find("[");

            if (left_bracket != string::npos) {
                tag = path.substr(0, left_bracket);
                selector = path.substr(left_bracket + 1, path.length() - left_bracket - 2);

                if (isnum(selector))
                    index = stoi(selector);
                else {
                    stringstream ss;
                    ss.str(selector);
                    while (getline(ss, selector, ',')) {
                        selectors.push_back(selector);
                    }
                }
            }

            auto pos = s.find("<" + tag);

            while (pos != string::npos) {
                int start_l, start_r, end_l, end_r, count;

                start_l = (int) pos;
                start_r = start_l;
                while (start_r < s.length() && s[start_r] != '>')
                    start_r++;

                count = 1;
                end_l = start_r;

                while (count) {
                    end_l = (int) s.find(tag, end_l + 1);
                    if (end_l <= start_r) {
                        break;
                    }

                    if (s[end_l - 2] == '<' && s[end_l - 1] == '/')
                        count--;
                    if (s[end_l - 1] == '<')
                        count++;
                }

                if (end_l <= start_r) {
                    end_r = start_r;
                } else {
                    end_l = end_l - 2;
                    end_r = end_l + (int) tag.length() + 1;
                }

                pos = s.find("<" + tag, end_r + 1);

                bool match = true;

                for (string selector:selectors) {
                    if (s.substr(start_l, start_r - start_l + 1).find(selector) == string::npos) {
                        match = false;
                        break;
                    }
                }


                if (!match)
                    continue;
                if (isLast) {
                    if (end_r - start_l + 1 >= 0)
                        res.push_back(s.substr(start_l, end_r - start_l + 2));
                } else {
                    if (end_l - start_r - 1 >= 0)
                        res.push_back(s.substr(start_r + 1, end_l - start_r - 1));
                }

            }

            if (index != -1) {
                ParserObj selected = res[index];
                res.clear();
                res.push_back(selected);
            }

            return res;
        }

        vector<ParserObj> find_by_xpath(string xpath) {

            vector<ParserObj> res;
            res.push_back(*this);

            vector<string> paths;
            string path;

            bool inreadingselector;
            inreadingselector = false;

            int start = 0;

            for (int i = 1; i < xpath.size(); i++) {
                if (xpath[i] == '/' && !inreadingselector) {
                    paths.push_back(xpath.substr(start, i - start));
                    start = i + 1;
                }
                if (xpath[i] == '[')
                    inreadingselector = true;
                if (xpath[i] == ']')
                    inreadingselector = false;
            }

            if (start < xpath.length())
                paths.push_back(xpath.substr(start));

            vector<ParserObj> tmp;

            for (int i = 0; i < paths.size(); i++) {
                tmp.clear();
                for (int j = 0; j < res.size(); j++) {
                    vector<ParserObj> tmp2 = res[j].extract( paths[i], i == paths.size() - 1);
                    tmp.insert(tmp.end(), tmp2.begin(), tmp2.end());
                }
                res = tmp;
            }

            return res;
        }


        vector<ParserObj> find_all(string tag, string selectors) {
            vector<ParserObj> res;

            if (tag.empty())
                return res;

            if (!selectors.empty())
                selectors = "[" + selectors + "]";

            string path = tag;

            vector<ParserObj> tmp;
            vector<ParserObj> tmpwithselector;
            tmp = find_by_xpath(path);
            while (!tmp.empty()) {
                tmpwithselector = find_by_xpath(path + selectors);
                res.insert(res.end(), tmpwithselector.begin(), tmpwithselector.end());
                path += "/" + tag;
                tmp = find_by_xpath(path);
            }
            return res;
        }

        ParserObj find_one(string tag, string selectors) {
            auto v = find_all(tag,  selectors);
            if (v.size()>0)
                return v[0];
            else
                return ParserObj("");
        }

        string to_string(){
            return s;
        }


    };
    namespace Parser {
        bool isnum(string s) {
            for (char c:s)
                if (!isdigit(c))
                    return false;
            return true;
        }

        string getAttribute(string html, string attribute) {
            string res = "";
            attribute = attribute + "=\"";
            auto pos = html.find(attribute);
            if (pos != string::npos) {
                pos = pos + attribute.length();
                for (auto i = pos; i < html.length(); i++) {
                    if (html[i] == '"') {
                        res = html.substr(pos, i - pos);
                        break;
                    }
                }
            }

            return res;
        }

        string getInnerHTML(string html) {
            int start, end;
            start = 0;
            end = (int) html.size() - 1;

            while (html[start] != '>')
                start++;

            while (html[end] != '<')
                end--;

            if (start >= end)
                return "";
            else
                return html.substr(start + 1, end - start - 1);

        }


        vector<string> extract(string src, string path, bool isLast) {

            vector<string> res;
            vector<string> selectors;
            string tag, selector;
            int index;
            bool isvoid;

            isvoid = false;
            tag = path;
            index = -1;

            auto left_bracket = path.find("[");

            if (left_bracket != string::npos) {
                tag = path.substr(0, left_bracket);
                selector = path.substr(left_bracket + 1, path.length() - left_bracket - 2);

                if (isnum(selector))
                    index = stoi(selector);
                else {
                    stringstream ss;
                    ss.str(selector);
                    while (getline(ss, selector, ',')) {
                        selectors.push_back(selector);
                    }
                }
            }

            auto pos = src.find("<" + tag);

            while (pos != string::npos) {
                int start_l, start_r, end_l, end_r, count;

                start_l = (int) pos;
                start_r = start_l;
                while (start_r < src.length() && src[start_r] != '>')
                    start_r++;

                count = 1;
                end_l = start_r;

                while (count) {
                    end_l = (int) src.find(tag, end_l + 1);
                    if (end_l <= start_r) {
                        break;
                    }

                    if (src[end_l - 2] == '<' && src[end_l - 1] == '/')
                        count--;
                    if (src[end_l - 1] == '<')
                        count++;
                }

                if (end_l <= start_r) {
                    end_r = start_r;
                } else {
                    end_l = end_l - 2;
                    end_r = end_l + (int) tag.length() + 1;
                }

                pos = src.find("<" + tag, end_r + 1);

                bool match = true;

                for (string selector:selectors) {
                    if (src.substr(start_l, start_r - start_l + 1).find(selector) == string::npos) {
                        match = false;
                        break;
                    }
                }


                if (!match)
                    continue;
                if (isLast) {
                    if (end_r - start_l + 1 >= 0)
                        res.push_back(src.substr(start_l, end_r - start_l + 2));
                } else {
                    if (end_l - start_r - 1 >= 0)
                        res.push_back(src.substr(start_r + 1, end_l - start_r - 1));
                }

            }

            if (index != -1) {
                string selected = res[index];
                res.clear();
                res.push_back(selected);
            }

            return res;
        }

        vector<string> find_by_xpath(string src, string xpath) {

            vector<string> res;
            res.push_back(src);

            vector<string> paths;
            string path;

            bool inreadingselector;
            inreadingselector = false;

            int start = 0;

            for (int i = 1; i < xpath.size(); i++) {
                if (xpath[i] == '/' && !inreadingselector) {
                    paths.push_back(xpath.substr(start, i - start));
                    start = i + 1;
                }
                if (xpath[i] == '[')
                    inreadingselector = true;
                if (xpath[i] == ']')
                    inreadingselector = false;
            }

            if (start < xpath.length())
                paths.push_back(xpath.substr(start));

            vector<string> tmp;

            for (int i = 0; i < paths.size(); i++) {
                tmp.clear();
                for (int j = 0; j < res.size(); j++) {
                    vector<string> tmp2 = extract(res[j], paths[i], i == paths.size() - 1);
                    tmp.insert(tmp.end(), tmp2.begin(), tmp2.end());
                }
                res = tmp;
            }

            return res;
        }


        vector<string> find_all(string src, string tag, string selectors) {
            vector<string> res;

            if (tag.empty())
                return res;

            if (!selectors.empty())
                selectors = "[" + selectors + "]";

            string path = tag;

            vector<string> tmp;
            vector<string> tmpwithselector;
            tmp = find_by_xpath(src, path);
            while (!tmp.empty()) {
                tmpwithselector = find_by_xpath(src, path + selectors);
                res.insert(res.end(), tmpwithselector.begin(), tmpwithselector.end());
                path += "/" + tag;
                tmp = find_by_xpath(src, path);
            }
            return res;
        }

        string find_one(string src, string tag, string selectors) {
            auto v = find_all( src,  tag,  selectors);
            if (v.size()>0)
                return v[0];
            else
                return "";
        }

    }
}
