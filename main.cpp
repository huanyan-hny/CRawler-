
#include <future>
#include <iostream>
#include <atomic>
#include <mutex>
#include <queue>
#include "Engine.h"
using namespace std;
using boost::asio::ip::tcp;
using namespace Crawler;


enum { max_length = 1024 };
int main()
{
//	Crawler::Engine<Crawler::Generic_Scheduler> e (1,5);
//	e.start();
// code above is working, regardless the network issue.
    auto a = Crawler::Downloader::sync_download(make_shared<Crawler::Request>(Crawler::Request("get","www.imdb.com/title/tt0796366/")));

    ofstream ofs;
    ofs.open("test.txt", std::ofstream::out);
    ofs << a->asio_response;

    ofs.close();

    return 0;





}