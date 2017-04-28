
#include <future>
#include <iostream>
#include <atomic>
#include <mutex>
#include <queue>

#include "HTML.h"
#include "Engine.h"

#include "demo/IMDB/IMDBSpider.h"
#include "demo/IMDB/IMDBItemPipeline.h"

#include "demo/GrubHub/GrubhubItemPipeline.h"
#include "demo/GrubHub/GrubhubSpider.h"

using namespace std;


void run_IMDB(){
    shared_ptr<Crawler::IMDBSpider> spider = make_shared<Crawler::IMDBSpider>();
    shared_ptr<Crawler::IMDBItemPipeline> pipeline = make_shared<Crawler::IMDBItemPipeline>();
    Crawler::Engine<Crawler::Generic_Scheduler> e(spider,pipeline);
    e.set_max_threads(5);
    e.set_iterations(3);
    e.start();
}

void run_GrubHub(){
    auto spider = make_shared<Crawler::GrubhubSpider>();
    auto pipeline = make_shared<Crawler::GrubhubItemPipeline>();
    Crawler::Engine<Crawler::Generic_Scheduler> e(spider,pipeline);
    e.set_max_threads(5);
    e.set_iterations(3);
    e.start();
}

int main()
{
//    run_IMDB();
    run_GrubHub();
    return 0;

}


