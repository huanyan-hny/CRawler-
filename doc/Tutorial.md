# Tutorial: IMDB Spider

This step-by-step tutorial provides an example of how to use CrawlerPlusPlus to crawl for websites with static HTML as well as grab pictures from websites.

## CrawlerPlusPlus Features Used
- FILE
- HTML Parser
## Problem Overview

In this tutorial we will build a spider that will extract data (restaurant name, dishes, price and whether popular) from GrubHub.


## Step 1 Examine the website to be crawled:

We start by visiting one movie introduction page: http://www.imdb.com/title/tt0796366/
We are going to grab the following data: the name of the moive, the rating of the movie, the number of people who rated the move as well as the poster of the movie. Of course, we need to find the other movies which people like this movie also like, get the link of these movies, send them to the next iteration. 

By looking into the HTML source file, we find that everything we need can be found in the static source file. That’s good, getting them is not a big deal. Then we tried some more links in IMDB, we find that all of the page have to same layout, which means we can use the same logic to handler different pages. At last, we get the url of the poster and try to access it by our browser: https://images-na.ssl-images-amazon.com/images/M/MV5BMjE5NDQ5OTE4Ml5BMl5BanBnXkFtZTcwOTE3NDIzMw@@._V1_UX182_CR0,0,182,268_AL_.jpg
We find that the picture could be showed in our browser, which means that it could also be downloaded by our downloaded. 
Cool! We can grap all of them by our Spider then.


## Step 2: Write the spider!

OK! Time to write our spider. We write a class called IMDBSpider, inheriting the Spider class and write our own logical. That’s the design pattern in CRawlerPlusPlus. 

By looking into the documentation, we find that there is one pure virtual function we need to implement:

    void initial_tasks(function<void(Task&)> add_task);

In this method, we need to define the initial tasks. Here we choose the Star Trek as the initial page. So we form a new Task:

    Task("www.imdb.com/title/tt0796366/?ref_=tt_rec_tti",callback(&IMDBSpider::parse_link)).put_string("type","link")

The first param is the link of the movie. The second param is the callback function, which will be called after the engine downloaded the web page to you. As the first link is the HTML page, not the image, we’d like to associate it with the parse_link() function. Then we can bind some data to the task to help us parse the HTTP response. At last we need to use the callback funciton add_task() to add the task.

Then we need to write two handler functions: parse_link() and parse_image(). One for the movie HTML, another one for the movie poster. 

First we start by writing the parse_link() function. In the parse_link() function, we parse the HTML, extract data we want. We are able to parse the HTML by the CRawlerPlusPlus HTML parser. Here is the example how to extract the movie file from IMDB.


https://d2mxuefqeaa7sj.cloudfront.net/s_C0E6EEEFB844157C492CFACAE66E3EF811A556050B6EF424C38AE4974A57D008_1493358878951_file.png


By looking into the HTML source file, we find that the name of the moive lies on one meta type element, which have a unique property ‘og:title’. We can find this element first and the get the content of the ‘content’ attribute, which is exactly the name of the flim. We can extract it with the following code. Other fields could be extracted easily also.


            auto movie_name = ParserObj(content).find_one("meta", "og:title").getAttribute("content").s;

Extracting related movies seems to be more difficult because there are more than one related movies. But don’t worry, the find_all() method helps you do that!


    auto related_movie_urls = ParserObj(content).find_all( "a", "ref_=tt_rec_tti");

After extracting all of the fields we want. We can produce the item of this movie.


                i.type = "movie";
                i["movie_name"] = movie_name;
                i["movie_url"] = task->get_url();
                i["rating"] = rating;
                i["rating_count"] = rating_count;
                produce_item(i);

Consider the produce_item() callback as yield return, it’s quite easy to understand!

Then we need to add new tasks which we expect the crawler to grab them later. In this case, it’s quite tricky becuase we have two kinds of tasks. First we need to let the system to grab the page of related movies. Second we need to download the poster of the current movie. We dispatch them to two different handlers. Movie pages will be routed back to the same handler we are writing, while we want to use antoher handler to handler poster.s 

                    add_task(Task(url_link, callback(&IMDBSpider::parse_link));
                    add_task(Task(movie_poster_link, callback(&IMDBSpider::parse_image), Request_method::GET, Request_content::FILE));

When we add new tasks for the poster, we need to explicitly set the Request_content as FILE so that the download could treat it as a binary FILE rather than a string. 

Parse_image() function is quite simple, we just need to produce the file we download. We set the filename to the name of the movie, and set the extension to jpg. At last call the produce_item() call back to produce it.

            Item i;
            i.type = "poster";
            i.name = task->bundle["movie_name"];
            i.feed_file("poster",res->content,"jpg");
            produce_item(i);

Here is the complete version of IMDBSpider.h.


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
                    add_task(Task(movie_poster_link, callback(&IMDBSpider::parse_image), Request_method::GET, Request_content::FILE).put_string("type","poster").put_string("movie_name",movie_name).ignore_iteration_limit(true));
                    for (auto &url: related_movie_urls) {
                        auto url_link = domains + getAttribute(url.s, "href");
                        add_task(Task(url_link, callback(&IMDBSpider::parse_link)).
                                put_string("type","link"));
                    }
                }
            }
        };
    }


## Step 3: Write the item pipeline!

For our task the item pipeline is fairly easy. Create a file called IMDBItemPipeline.h


    #pragma once
    #include <string>
    #include <vector>
    #include <unordered_map>
    #include <assert.h>
    #include <iostream>
    #include <fstream>
    #include "Item.h"
    #include "ItemPipeline.h"
    #include <boost/filesystem.hpp>
    using namespace std;
    
    namespace Crawler
    {
           class IMDBItemPipeline: public ItemPipeline
            {
            public:
            ofstream outputer;
                    void open_spider(shared_ptr<Spider> spider) {
                string filename =  "./output/movies.csv";
                outputer.open(filename, std::ofstream::out);
            }
                    void close_spider(shared_ptr<Spider> spider) {
                outputer.close();
            }
                    void process_item(shared_ptr<Item> item)
            {
                if (item->type == "movie") {
                    static bool first = true;
                    if (first) {
                        outputer << item->to_csv_title() << endl;
                        first = false;
                    }
                    outputer << item->to_csv() << endl;
                }
                else if (item->type == "poster")
                    simple_file_write("./output",item->data["poster"],item->name);
                    }
    
            };
    }


- The open_spider is invoked once spider starts to run, and close_spider is invoked when closed. They are usually doing quite standard things, such as opening / closing files, as is in this case.


- Remember that we set the type field of poster item to “poster” and movie information item to “movie”.  Now we are able to use that value to apply different logic on different items. If it is a moive, we will simply output the item to csv. Item::to_csv_title() and Item::to_csv() function help us do that easily. If it is a poster, we write this poster to hard disk. The predefined simple_file_write() function helps us to write the file.



## Step 4: Make the crawler work!
    #include "demo/IMDB/IMDBItemPipeline.h"
    #include "demo/IMDB/IMDBSpider.h"
    
    using namespace std;
    
    int main() {
        auto spider = make_shared<Crawler::IMDBSpider>();
        auto pipeline = make_shared<Crawler::IMDBItemPipeline>();
        Crawler::Engine e(spider,pipeline); // link engine with spider and pipeline
        e.set_max_threads(5);
        e.set_iterations(3); // set how deep to go
        e.start();
        return 0;
    }

Cool! It’s time to write the main function. We create two instances of spider and item pipeline in the main function,  pass them to the engine. We don’t want to grab too many movies so we set the iteration level to 3. To accelatrate the crawling process, we make the engine work in multithreaded mode. Then we are able to start the engine to collect data.


https://d2mxuefqeaa7sj.cloudfront.net/s_C0E6EEEFB844157C492CFACAE66E3EF811A556050B6EF424C38AE4974A57D008_1493362171267_file.png


After the crawler finishes, we can found posters as well as the csv in the output directory.

# Advanced Tutorial: GrubHub Spider

This step-by-step tutorial provides an example of how to use CrawlerPlusPlus to crawl for websites with dynamic generated HTML.

## CrawlerPlusPlus Features Used
- Authentication (OAuth 2.0)
- Session
- POST requests
- JSON Parser


## Problem Overview

In this tutorial we will build a spider that will extract data (restaurant name, dishes, price and whether popular) from GrubHub.


## Step 1 Examine the website to be crawled:

We start by visiting https://www.grubhub.com, and type in our location: The url got will be our very first task:


https://d2mxuefqeaa7sj.cloudfront.net/s_3C499542027FFD3AA1572A1B4FFE14D07B63AA24D6438035D35F80177285D14D_1493349050849_Screen+Shot+2017-04-27+at+11.10.40+PM.png


Click on view source and we found that the HTML is **dynamically generated**, meaning the JavaScript will render HTML and CSS on the fly:


            <!DOCTYPE html><html id="ng-app" ng-controller="SiteController as site"><head ghs-page-meta><link rel="dns-prefetch" href="//ajax.googleapis.com/"><link rel="dns-prefetch" href="//clickstream.grubhub.com/"><link rel="dns-prefetch" href="//api-gtm.grubhub.com"><link rel="dns-prefetch" href="//dtyxqspugqu5z.cloudfront.net"><meta http-equiv="X-UA-Compatible" content="IE=edge"><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no"><meta name="expires" content="-1"><meta http-equiv="Cache-Control" content="no-cache"><meta name="msapplication-config" content="none"><meta name="fragment" content="!"><meta name="prerender-status-code" content="{{site.pageMetaService.pageMeta.preRenderStatusCode}}"><meta ng-if="site.pageMetaService.pageMeta.preRenderHeader" name="prerender-header" content="Location: {{site.pageMetaService.pageMeta.preRenderHeader}}"><title ng-bind="site.pageMetaService.pageMeta.title"></title><meta name="description" content="{{site.pageMetaService.pageMeta.desc}}"><meta ghs-meta-tag="meta" ng-repeat="meta in site.pageMetaService.pageMeta.metas track by $id(meta)"><link ghs-link-attributes="link" ng-repeat="link in site.pageMetaService.pageMeta.links track by $id(link)"><link rel="canonical" href="{{site.pageMetaService.pageMeta.url}}"><base href="/"><script src="https://cdn.optimizely.com/js/2323050639.js"></script><link rel="stylesheet" href="https://assets.grubhub.com/css/site-2d2f53d14d90d510a304741d823834a9.css"/><link rel="stylesheet" href="https://assets.grubhub.com/css/commons-74d99c2b84798e161e3d64896593c9b6.css"/><link rel="shortcut icon" type="image/x-icon" href="https://assets.grubhub.com/favicon.ico"/><style>[ng-cloak] {
            display: none;
            }
            
            .splash {
            font-size: 2em;
            text-align: center;
            margin-top: 15%;
            display: none;
            }
            
            [ng-cloak].splash {
            display: block;
            }</style></head><body id="Site" ng-class="{
            'contentOnly' : site.contentOnly,
            'body-cartIsOpen' : site.isCartOpen(),
            'closedBagExperiment' : site.isClosedBagEnabled()
            }"><div ng-if="::(false)" class="siteLoader"><div class="siteLoader-content"><div class="loadingPanel"><div class="loadingPanel-content"><div class="loadingPanel-animation"><div class="floatingCircles"><div class="circle circle-unit-01"></div><div class="circle circle-unit-02"></div><div class="circle circle-unit-03"></div><div class="circle circle-unit-04"></div><div class="circle circle-unit-05"></div><div class="circle circle-unit-06"></div><div class="circle circle-unit-07"></div><div class="circle circle-unit-08"></div></div></div></div></div></div></div><a id="scroll-top" class="scroll-top" ghs-scrollspy></a><ghs-keep-scroll-position></ghs-keep-scroll-position><div class="mainNav" ng-if="::!site.contentOnly"></div><ghs-main-nav ng-if="::!site.contentOnly" class="mainNav"></ghs-main-nav><div class="outerWrapper clearfix" id="ghs-outerWrapper" ng-class="{'contentOnly' : site.contentOnly }" ng-cloak><div id="ghs-alertArea" cs-fires-on-load="site-loaded" class="notifications-container" ng-include="'shared/components/notifications/notification-container.tpl.html'"></div><div class="innerWrapper clearfix content-transition" ng-cloak><ghs-loading></ghs-loading><ghs-error></ghs-error><ng-view></ng-view></div><div id="ghs-globalCart-container" class="globalCart-container s-col-xs-12 s-col-md-5 s-col-lg-3 s-pull-right" ng-if="(site.isCartOpen() || site.isCartAlwaysOpen())" ng-class="{ 'isAlwaysOpen' : site.isCartAlwaysOpen(), 'isOpen' : site.isCartOpen() }"><div ghs-global-cart ng-if="site.cartIsReady"></div></div><ghs-pending-review ng-if="site.showPendingReview"></ghs-pending-review></div><ghs-modal-launcher></ghs-modal-launcher><div ng-if="site.toasterTemplateUrl" ng-include="site.toasterTemplateUrl"></div><ghs-tour-bubble ng-if="site.showTourBubble"></ghs-tour-bubble><div id="fb-root"></div><script>!function(e){function r(n){if(t[n])return t[n].exports;var o=t[n]={i:n,l:!1,exports:{}};return e[n].call(o.exports,o,o.exports,r),o.l=!0,o.exports}var n=window.webpackJsonp;window.webpackJsonp=function(t,u,c){for(var i,a,s,f=0,l=[];f<t.length;f++)a=t[f],o[a]&&l.push(o[a][0]),o[a]=0;for(i in u)Object.prototype.hasOwnProperty.call(u,i)&&(e[i]=u[i]);for(n&&n(t,u,c);l.length;)l.shift()();if(c)for(f=0;f<c.length;f++)s=r(r.s=c[f]);return s};var t={},o={16:0};r.e=function(e){function n(){c.onerror=c.onload=null,clearTimeout(i);var r=o[e];0!==r&&(r&&r[1](new Error("Loading chunk "+e+" failed.")),o[e]=void 0)}if(0===o[e])return Promise.resolve();if(o[e])return o[e][2];var t=new Promise(function(r,n){o[e]=[r,n]});o[e][2]=t;var u=document.getElementsByTagName("head")[0],c=document.createElement("script");c.type="text/javascript",c.charset="utf-8",c.async=!0,c.timeout=12e4,r.nc&&c.setAttribute("nonce",r.nc),c.src=r.p+window.webpackManifest[e];var i=setTimeout(n,12e4);return c.onerror=c.onload=n,u.appendChild(c),t},r.m=e,r.c=t,r.i=function(e){return e},r.d=function(e,n,t){r.o(e,n)||Object.defineProperty(e,n,{configurable:!1,enumerable:!0,get:t})},r.n=function(e){var n=e&&e.__esModule?function(){return e.default}:function(){return e};return r.d(n,"a",n),n},r.o=function(e,r){return Object.prototype.hasOwnProperty.call(e,r)},r.p="https://assets.grubhub.com/",r.oe=function(e){throw console.error(e),e}}([]);</script><script src="https://assets.grubhub.com/grubhub-config-tpblxisqmreqquyo34oqrq.js"></script><script src="https://assets.grubhub.com/grubhub-variants-nplainzuogdnzdgwfft8cw.js"></script><script src="https://ajax.googleapis.com/ajax/libs/angularjs/1.6.4/angular.min.js"></script><script src="https://assets.grubhub.com/js/vendor-171ab6a49c456b9d43b6.js"></script><script src="https://assets.grubhub.com/js/commons-59b2dfdc1e805e55e16a.js"></script><script src="https://assets.grubhub.com/js/root-b44d8ef98fd7abb25f22.js"></script><script src="https://assets.grubhub.com/resource-text/root-b44d8ef98fd7abb25f2227e932b113ca.js"></script><script async>function currentDate(e){var t=new Date,n=t.getDate(),a=t.getMonth()+1,u=t.getFullYear();document.getElementById(e).value=a+"/"+n+"/"+u}</script></body></html>

This is a bad news: It is sort-of an anti-spider thing because if we just naively request the HTML, we get nothing we want.

But it has to come from somewhere! Instead, let’s use Chrome’s developer tool and check the network behavior of the browser starting from the page loading. Interestingly, we get this:


https://d2mxuefqeaa7sj.cloudfront.net/s_3C499542027FFD3AA1572A1B4FFE14D07B63AA24D6438035D35F80177285D14D_1493349373121_Screen+Shot+2017-04-27+at+11.15.33+PM.png


This is exactly what we want! Let’s now look at the HTTP request sent by Chrome:


https://d2mxuefqeaa7sj.cloudfront.net/s_3C499542027FFD3AA1572A1B4FFE14D07B63AA24D6438035D35F80177285D14D_1493349522422_Screen+Shot+2017-04-27+at+11.18.17+PM.png


One thing we notice: there is an authorization section, and since the method is “Bearer”, it is using OAuth2.0 with a token! Let’s jot down the token for future use in our spider.


## Step 2: Write the spider!

Now we are gathered enough information to start. Create a new file called GrubhubSpider.h and include the following lines:


    #include <string>
    #include <vector>
    #include <memory>
    #include "Spider.h"
    #include "Json.h"
    
    using namespace std;
    
    namespace Crawler {
        using namespace Parser;
        class GrubhubSpider: public Spider { // Need to inherit Spider base case
        public:
            void initial_tasks(function<void(Task&)> add_task) {
                string token = "YOUR_TOKEN_HERE";
                string base_url = "https://api-gtm.grubhub.com/restaurants/search?location=POINT(-73.96379853%2040.79992675)";
                add_task(Task(base_url,
                              callback(&GrubhubSpider::parse_restaurant),
                              Auth_type::OAUTH2,
                              Authentication("Bearer",token))
                              .put_string("auth",token));
            }
    
            void parse_restaurant(shared_ptr<Task> task, shared_ptr<Response> res,
                    function<void(Task&)> add_task, function<void(Item&)> produce_item) {
    
                string json_str = res->asio_response;
                vector<Json> ids = Json::parse(json_str).find_by_key("restaurant_id");     
                string url = "https://api-gtm.grubhub.com/restaurants/";
                for (auto &id: ids) {
                    add_task(Task(url+id.getString(),
                                  callback(&GrubhubSpider::parse_menu),
                                  Request_method::POST,
                                  Auth_type::OAUTH2,
                                  Authentication("Bearer",task->bundle["auth"]),
                                  Form ({{"hideUnavailableMenuItems","true"}}))
                                  .put_string("id",id.getString()));
                }
            }
    
            void parse_menu(shared_ptr<Task> task, shared_ptr<Response> res,
                    function<void(Task&)> add_task, function<void(Item&)> produce_item){
                Item item;
                Json j = Json::parse(res->asio_response);
                string restaurant = j.find_by_key("restaurant")[0]
                                     .find_by_key("name")[0].getString();
                vector<Json> result = j.find_by_key("menu_category_list");
                vector<Json> list = result[0].find_by_key("menu_item_list");
                for (int j = 0; j < list.size(); j++) {
                    Json menu = list[j];
                    for (int i = 0; i < menu.size(); i++) {
                        item.type = "menu";
                        item.name = task->bundle["id"];
                        item["restaurant"] = restaurant;
                        item["name"] =  menu[i]["name"].toString();
                        item["price"] = menu[i]["price"]["amount"].toString();
                        item["popular"] = menu[i]["popular"].getBool() ? "yes" : "no";
                        produce_item(item);
                    }
                }
            }
        };
    }

As we can see, the code is quite short and clean:


- We start with initial_tasks, which is our base url mentioned in Step 0. we create a Task and use yield-like add_task to push it into the task queue. Note that how easy it is to specify Authentication method and details.
- The task for base_url has a call-back to parse_restaurant. Now we implement that. The argument list for parse functions is all the same, task, res, add_task and produce_item.
- In there we use our **in-house JSON parser** to parse the JSON and get restaurant ids. We create another layer of Tasks (restaurant specific) and link the callback to parse_menu.
- We then implement parse_menu, where we do similar JSON parsing to extract out restaurant name, menu, price and whether popular.
- We create an item and keep rewriting it and send it through produce_item function. We use the task→bundle to retrieve information from previous store (parse_restaurant).


## Step 3: Write the item pipeline!

For our task the item pipeline is fairly easy. Create a file called GrubhubItemPipeline.h


    #include <string>
    #include <vector>
    #include <unordered_map>
    #include <assert.h>
    #include <iostream>
    #include <fstream>
    #include "Item.h"
    #include "ItemPipeline.h"
    #include <boost/filesystem.hpp>
    
    using namespace std;
    
    namespace Crawler
    {
        class GrubhubItemPipeline: public ItemPipeline
        {
        public:
            ofstream outputer;
            void open_spider(shared_ptr<Spider> spider) {
                string filename =  "./restaurant.csv";
                outputer.open(filename, std::ofstream::out);
            }
            void close_spider(shared_ptr<Spider> spider) {
                outputer.close();
            }
            void process_item(shared_ptr<Item> item)
            { 
                if (item->type == "menu")
                    outputer << item->to_csv() << endl;
            }
    
        };
    }


- The open_spider is invoked once spider starts to run, and close_spider is invoked when closed. They are usually doing quite standard things, such as opening / closing files, as is in this case.
- Yes, the process_item is the process_item you called in Step 2. Here, we check if item→type is menu. If so, we will simply output the item to csv.
- The item type check here is redundant, since we only have one type, but this is the general way of how you differentiate items by item type and do different things.


## Step 4: Spider awakens!

Now we can start things! In main.cpp:

    #include "demo/GrubHub/GrubhubItemPipeline.h"
    #include "demo/GrubHub/GrubhubSpider.h"
    
    using namespace std;
    
    int main() {
        auto spider = make_shared<Crawler::GrubhubSpider>();
        auto pipeline = make_shared<Crawler::GrubhubItemPipeline>();
        auto e(spider,pipeline); // link engine with spider and pipeline
        e.set_max_threads(5);
        e.set_iterations(3); // set how deep to go
        e.start();
        return 0;
    }

And that’s it! Now you will be able to choose what you want to eat by simply sorting and viewing all kinds of food on GrubHub near you! 🍕 🍱 😃
