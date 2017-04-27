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

            auto movie_name = ParserObj(content).find_one("meta", "og:title").getAttribute("content").s;
            auto related_movie_names = ParserObj(content).find_all("img", "height=\"113\", width=\"76\"");
            auto related_movie_urls = ParserObj(content).find_all( "a", "ref_=tt_rec_tti");
            auto movie_poster_link = ParserObj(content).find_one("img", "Poster,itemprop=\"image\"").getAttribute("src").s;

            string rating = ParserObj(content).find_one("span","ratingValue").getInnerHTML().s;
            string rating_count = ParserObj(content).find_one("span","ratingCount").getInnerHTML().s;

            cout<<"movie_poster_link " << movie_poster_link<<endl;
            if (movie_name != "") {
                i.type = "movie";
                i["movie_name"] = movie_name;
                i["movie_url"] = task->get_url();
                i["rating"] = rating;
                i["rating_count"] = rating_count;
                produce_item(i);
                for (auto &url: related_movie_urls) {
                    auto url_link = domains + getAttribute(url.s, "href");
                    add_task(Task(url_link, callback(&IMDBSpider::parse_link)).
                            put_string("type","link"));
                    add_task(Task(movie_poster_link, callback(&IMDBSpider::parse_image), Request_method::GET, Request_content::FILE)
                                     .put_string("type","poster").put_string("movie_name",movie_name).ignore_iteration_limit(true));

                }
            }
        }



    };
}