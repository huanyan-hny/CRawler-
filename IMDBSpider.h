#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Spider.h"

using namespace std;

namespace Crawler
{
    class IMDBSpider: public Spider
    {
    public:
        vector<shared_ptr<Request>> start_requests()
        {
            vector<shared_ptr<Request>> v;
            v.push_back(make_shared<Request>(Request("get","www.imdb.com/title/tt0796366/?ref_=tt_rec_tti",Request_content::STRING).
                    put_string("type","link")));
            domains = "www.imdb.com";
            return v;
        }
        Parsed_Result parse(shared_ptr<Request> req, shared_ptr<Response> res)
        {
            Parsed_Result ret;
            Item i;
            if (req->bundle["type"] == "link") {
                string content = res->asio_response;
                string movie_name = Parser::getAttribute(Parser::find_one(content, "meta", "og:title"),"content");
                vector<string> related_movie_names = Parser::find_all(content, "img", "height=\"113\", width=\"76\"");
                vector<string> related_movie_urls = Parser::find_all(content, "a", "ref_=tt_rec_tti");
                string movie_poster = Parser::find_one(content, "img", "Poster,itemprop=\"image\"");
                string movie_poster_link = Parser::getAttribute(movie_poster, "src");
                cout<<"movie_poster_link " << movie_poster_link<<endl;
                if (movie_name != "") {
                    i.type = "movie";
                    i["movie_name"] = Parser::getAttribute(movie_name, "content");
                    i["movie_url"] = req->get_url();
                    ret.items.push_back(make_shared<Item>(i));
                    for (auto &url: related_movie_urls) {
                        auto url_link = domains + Parser::getAttribute(url, "href");
                        ret.next_reqs.push_back(
                                make_shared<Request>(Request("get", url_link, Request_content::STRING).
                                        put_string("type","link")));
                        ret.next_reqs.push_back(make_shared<Request>(
                                Request("get", movie_poster_link, Request_content::FILE).
                                put_string("type","poster").put_string("movie_name",movie_name).set_ignore_iterating_limit(true)));

//                        cout << url_link << endl;
                    }
//                    cout << i.to_string();
                }
            }
            else if (req->bundle["type"] == "poster")
            {
                i.type = "poster";
                i.name = req->bundle["movie_name"];
                i.feed_file("poster",res->content,"jpg");
                ret.items.push_back(make_shared<Item>(i));
            }
            else
                cout<<"unknown request"<<endl;

            return ret;

        }

    };
}