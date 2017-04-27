
#include <future>
#include <iostream>
#include <atomic>
#include <mutex>
#include <queue>

#include "Parser.h"
#include "Engine.h"

#include "IMDBSpider.h"
#include "IMDBItemPipeline.h"

#include "GrubhubItemPipeline.h"
#include "GrubhubSpider.h"

using namespace std;




int main()
{
    shared_ptr<Crawler::IMDBSpider> spider = make_shared<Crawler::IMDBSpider>();
    shared_ptr<Crawler::IMDBItemPipeline> pipeline = make_shared<Crawler::IMDBItemPipeline>();
//    auto spider = make_shared<Crawler::GrubhubSpider>();
//    auto pipeline = make_shared<Crawler::GrubhubItemPipeline>();
	Crawler::Engine<Crawler::Generic_Scheduler> e(spider,pipeline);
    e.set_max_threads(5);
    e.set_iterations(3);
    e.start();
    return 0;


/*
    Request request("get","www.stroustrup.com/Bjarne.jpg", true);
    std::shared_ptr<Response> r_ptr = Downloader::Curl_Downloader::get(request);


    ofstream fout;
    fout.open("my_best_professor.jpg",ios::binary | ios::out);
    fout.write((char *)&(r_ptr->content[0]),r_ptr->content.size());


    fout.close();
*/
// code above is working, regardless the network issue.
    /*
    using namespace Crawler::Parser;

    auto content = Crawler::Downloader::sync_download(make_shared<Crawler::Request>(Crawler::Request("get","www.imdb.com/title/tt0796366/"))).get()->asio_response;
    vector<string> movie_names = find_all(content, "meta", "og:title");
    vector<string> related_movie_names = find_all(content,"img","height=\"113\", width=\"76\"");
    vector<string> related_movie_urls = find_all(content, "a", "ref_=tt_rec_tti");
    cout << getAttribute(movie_names[0], "content") << endl;
    cout << "Related movies:" << endl;
    for (int i=0;i<related_movie_names.size();i++) {
        cout << getAttribute(related_movie_names[i], "title") << ":  " << getAttribute(related_movie_urls[i], "href") << endl;
    }
    */

}


