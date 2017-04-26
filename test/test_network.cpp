#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <fstream>
#include "../Downloader.h"
#include "../Network.h"

using boost::asio::ip::tcp;
using namespace Crawler;


void test_request_basic() {

    Request request("get","www.imdb.com/title/tt0796366/", Request_content::STRING, "");
    std::shared_ptr<Response> r_ptr = Downloader::Boostasio_Downloader::get(request);
    cout << r_ptr->header;
}

void test_curl_download() {

    Request request("get","www.stroustrup.com/Bjarne.jpg", Request_content::FILE,"");
    std::shared_ptr<Response> r_ptr = Downloader::Curl_Downloader::get(request);
    ofstream fout;
    fout.open("my_best_professor.jpg",ios::binary | ios::out);
    fout.write((char *)&(r_ptr->content[0]),r_ptr->content.size());
    fout.close();

}

void test_curl_download_https(){

    Request request("get","https://images-na.ssl-images-amazon.com/images/M/MV5BMjE5NDQ5OTE4Ml5BMl5BanBnXkFtZTcwOTE3NDIzMw@@._V1_SY1000_CR0,0,674,1000_AL_.jpg", Request_content::FILE,"");
    std::shared_ptr<Response> r_ptr = Downloader::Curl_Downloader::get(request);
    ofstream fout;
    fout.open("star_trek.jpg",ios::binary | ios::out);
    fout.write((char *)&(r_ptr->content[0]),r_ptr->content.size());
    fout.close();
}

void test_curl_request_basic(){

    Request request("get","www.imdb.com/title/tt0796366/", Request_content::STRING, "");
    std::shared_ptr<Response> r_ptr = Downloader::Curl_Downloader::get(request);
    cout << r_ptr->asio_response;

}


void test_session_basic() {
//    Crawler::Session s {};
//    std::shared_ptr<Response> r_ptr = s.request("GET", "www.rottentomatoes.com/m/the_lost_city_of_z");
//    cout << r_ptr->header;
//    r_ptr = s.request("GET", "www.rottentomatoes.com/m/sleight");
//    cout << r_ptr->asio_response;

}

void test_curl_authentication() {
    Request request("get","xxx", Request_content::STRING, "",
                    Authentication{"access_token", "xxx"});
    std::shared_ptr<Response> r_ptr = Downloader::Curl_Downloader::get(request);
    cout << r_ptr->status_code;

}

int main(int argc, char* argv[])
{
    test_request_basic();
    test_curl_request_basic();
    test_curl_download();
    test_curl_download_https();
//    test_curl_authentication();
//    test_session_basic();

    return 0;
}


