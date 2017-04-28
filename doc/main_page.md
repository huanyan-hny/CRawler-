# Introduction

CrawlerPlusPlus is the Modern C++ solution for web crawling. CrawlerPlusPlus uses a multi-threaded, async I/O model powered by Boost ASIO and libcurl network backend to achieve blazingly fast, non-blocking HTTP connection. Together with Python-like user interface, CrawlerPlusPlus makes writing spider in C++ both easy and efficient.

# Design Principle

## KISS

The single most important design principle for CrawlerPlusPlus is to keep things simple. We believe that the more user writes, the more error-prone it will be. Therefore, we invested many efforts in simplifying and shortening the lines of code user has to write.

For example, once user has defined the spider and item pipeline to use, starting the engine is as simple as writing:


    auto e(spider,pipeline); // initialize engine with corresponding spider and pipeline
    e.set_max_threads(5);
    e.set_iterations(3);
    e.start(); // start to run the crawler!

With a simple `e.start( )` , we will handle all the “dirty work” for the user: scheduling tasks, downloading stuff, managing threads, create/destruct objects — it’s all happening behind-the-scene.

## Modularity

Simplicity should not be achieved with the sacrifice of extensibility. We want to give advanced users enough freedom to tune things as they like. The key to all these flexibility is our modular design, where, apart from the central piece — engine, all other components (spider, item pipeline, scheduler, etc.) are “invisible” to each other. Our system is designed in an object-oriented way which results in low decoupling and high cohesion.

## Take full advantage of modern C++ (Love it, but also fight against it)

We love many new features in modern C++ — for example, in the above code snippet we use `auto` to free user from writing explicit, long type name, resulting in ugly code.

Actually, in our project, you will see all kinds of modern C++ features floating around. To name a few:


- We firmly believe in RAII, so proper use of this library requires neither `new` nor `delete`  — all resources are cleaned up automatically as we fall out of scope, thanks to smart pointers.
- We extensively use std::async and related features to achieve non-blocking I/O, but do not need to deal with the nitty-gritty of thread management.

At the same time, we discovered during the process that there are things in C++ that have to be done in a not-so-beautiful way — for instance: implementing callback function (see Design Highlight 1). When it becomes so nature to do callback, we won’t compromise — we use C++’s expressiveness (esp. function object, bind and lambda) to create a callback framework that makes it so easy to use:


    add_task(Task("http://imdb.com/title/tt0796366/",callback(&IMDBSpider::parse_result));

Without callback framework it will be something look like this:


    class IMDBSpider: public Spider
    {
    public:
        void initial_tasks(function<void(Task&)> add_task)
        {
            add_task(Task("www.imdb.com/title/tt0796366/?ref_=tt_rec_tti", "parse_result");
        }
        
        void routing(shared_ptr<Task> task) {
            if (task->call_back == "parse_result")
              return parse_result(task);
            else if (task->call_back == "parse_image")
              return parse_image(task);
            else if (task->call_back == "parse_link")
              return parse_link(task);
            /* else if ....
             * Essentially you need to register and re-routing
             * every parser function you write! */
        }
        void parse_image(shared_ptr<Task> task) { /* Do work */ }
        void parse_link(shared_ptr<Task> task) { /* Do work */ }
        void parse_result(shared_ptr<Task> task) { /* Do work */ }
    }

It is lengthy, unnecessary and error-prone!

# Features

- Full fledged support to the majority of HTTP request functionality
	- GET request
	- POST request
	- Url encoded parameters
	- Url encoded POST values
	- Session
	- Cookie
	- Basic Authentication
	- OAuth 2.0
	- Custom Header
	- Multiple return type support (HTML, File or JSON)
	- HTTPS (with openssl)
- 100% user-friendly design
	- Flexible constructors with optional parameters that are **truly optional** (the sequence doesn't even matter!)
	- Painless callback function
	- Use function objects to achieve yield-like behavior
- Multithreaded solution with asynchronous I/O
	- But write the spider as if it was single-threaded -- we handle everything for you!
- Two downloader backends support
	- Boost ASIO downloader for simple, basic GET requests, but guarantees more low-level control;
	- Curl downloader for advanced usage, including POST requests, authentication, session, etc.
- In-house support for parsing and data extraction
	- Easily nevigate through both HTML and JSON files without the need of installing any third-party libraries!
- Simple, expressive yet extensive
	- Provided modular, pluggable scheduler, item pipeline and spider. Powerful default ones but you can also write your own!
- Basic anti-(anti-crawler) functionality
	- An intelligent scheduler that will match domain info and exclude duplicates automatically
	- Supports setting header for user-agent to 'trick' the server

# Authors

Yanlin Duan, Yimin Wei, Hongning Yuan
