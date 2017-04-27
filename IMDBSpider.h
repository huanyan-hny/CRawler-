#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Spider.h"

using namespace std;

namespace Crawler
{
    using namespace Parser;
    class IMDBSpider: public Spider
    {
    public:
        void initial_tasks(function<void(Task&)> add_task)
        {
            add_task(Task("www.imdb.com/title/tt0796366/?ref_=tt_rec_tti",callback(&IMDBSpider::parse_link)).
                    put_string("type","link"));
            domains = "www.imdb.com";
        }

        void parse_image(shared_ptr<Task> task, shared_ptr<Response> res,
                         function<void(Task&)> add_task, function<void(Item&)> produce_item)
        {
            Item i;
            i.type = "poster";
            i.name = task->bundle["movie_name"];
            i.feed_file("poster",res->content,"jpg");
            produce_item(i);
        }

        void parse_link(shared_ptr<Task> task, shared_ptr<Response> res,
                        function<void(Task&)> add_task, function<void(Item&)> produce_item)
        {
            Item i;
            string content = res->asio_response;
            string movie_name = getAttribute(find_one(content, "meta", "og:title"),"content");
            vector<string> related_movie_names = find_all(content, "img", "height=\"113\", width=\"76\"");
            vector<string> related_movie_urls = find_all(content, "a", "ref_=tt_rec_tti");
            string movie_poster = Parser::find_one(content, "img", "Poster,itemprop=\"image\"");
            string movie_poster_link = getAttribute(movie_poster, "src");
            string rating = Parser::getInnerHTML(Parser::find_one(content,"span","ratingValue"));
            string rating_count = Parser::getInnerHTML(Parser::find_one(content,"span","ratingCount"));
            cout<<"movie_poster_link " << movie_poster_link<<endl;
            if (movie_name != "") {
                i.type = "movie";
                i["movie_name"] = movie_name;
                i["movie_url"] = task->get_url();
                i["rating"] = rating;
                i["rating_count"] = rating_count;
                produce_item(i);
                for (auto &url: related_movie_urls) {
                    auto url_link = domains + getAttribute(url, "href");
                    add_task(Task(url_link, callback(&IMDBSpider::parse_link)).
                            put_string("type","link"));
                    add_task(Task(movie_poster_link, callback(&IMDBSpider::parse_image), Request_method::GET, Request_content::FILE)
                                     .put_string("type","poster").put_string("movie_name",movie_name).ignore_iteration_limit(true));

                }
            }
        }



    };
}