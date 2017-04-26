
#include <future>
#include <iostream>
#include <atomic>
#include <mutex>
#include <queue>

#include "Parser.h"
#include "Engine.h"

#include "IMDBSpider.h"
#include "IMDBItemPipeline.h"

using namespace std;


void test_curl_download_https(){
    using namespace Crawler;
    Request request("get","https://images-na.ssl-images-amazon.com/images/M/MV5BMjE5NDQ5OTE4Ml5BMl5BanBnXkFtZTcwOTE3NDIzMw@@._V1_SY1000_CR0,0,674,1000_AL_.jpg", Request_content::FILE,"");
    std::shared_ptr<Response> r_ptr = Downloader::Curl_Downloader::get(request);
    ofstream fout;
    fout.open("star_trek.jpg",ios::binary | ios::out);
    fout.write((char *)&(r_ptr->content[0]),r_ptr->content.size());
    fout.close();

}

int main()
{
    shared_ptr<Crawler::IMDBSpider> spider = make_shared<Crawler::IMDBSpider>();
    shared_ptr<Crawler::IMDBItemPipeline> pipeline = make_shared<Crawler::IMDBItemPipeline>();
	Crawler::Engine<Crawler::Generic_Scheduler> e(spider,pipeline);
    e.set_max_threads(5);
    e.set_maxium_layer(2);
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


