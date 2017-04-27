#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <fstream>
#include "../Downloader.h"
#include "../Network.h"
#include <cpr/cpr.h>

using boost::asio::ip::tcp;
using namespace Crawler;


void test_request_basic() {

    Task tsk("www.imdb.com/title/tt0796366/", nullptr);
    Downloader::Boostasio_Downloader bd;
    std::shared_ptr<Response> r_ptr = bd.get(tsk);
    cout << r_ptr->asio_response;
}

void test_curl_download() {

    Task tsk("www.stroustrup.com/Bjarne.jpg", nullptr, Request_method::GET, Request_content::FILE);
    Downloader::Curl_Downloader cd;
    std::shared_ptr<Response> r_ptr = cd.get(tsk);
    ofstream fout;
    fout.open("my_best_professor.jpg",ios::binary | ios::out);
    fout.write((char *)&(r_ptr->content[0]),r_ptr->content.size());
    fout.close();

}

void test_curl_download_https(){

    Task tsk("https://images-na.ssl-images-amazon.com/images/M/MV5BMjE5NDQ5OTE4Ml5BMl5BanBnXkFtZTcwOTE3NDIzMw@@._V1_SY1000_CR0,0,674,1000_AL_.jpg", nullptr, Request_method::GET, Request_content::FILE);
    Downloader::Curl_Downloader cd;
    std::shared_ptr<Response> r_ptr = cd.get(tsk);
    ofstream fout;
    fout.open("star_trek.jpg",ios::binary | ios::out);
    fout.write((char *)&(r_ptr->content[0]),r_ptr->content.size());
    fout.close();

}

size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}


void test_curl_request_basic(){

    Task tsk("www.imdb.com/title/tt0796366/", nullptr);
    Downloader::Curl_Downloader cd;
    std::shared_ptr<Response> r_ptr = cd.get(tsk);
    cout << r_ptr->asio_response;

}

void test_curl_auth2(){
    Task tsk("https://api-gtm.grubhub.com/restaurants/74367",nullptr,Auth_type::OAUTH2,Authentication("Bearer","bdd5ff4e-cfbe-4b03-818b-2d3b5fa974ec"));
    Downloader::Curl_Downloader cd;
    std::shared_ptr<Response> r_ptr = cd.get(tsk);
    cout << r_ptr->asio_response;
}
//
//
//void test_session_basic() {
//    Crawler::Session s {};
//    std::shared_ptr<Response> r_ptr = s.request("GET", "www.rottentomatoes.com/m/the_lost_city_of_z");
//    cout << r_ptr->header;
//    r_ptr = s.request("GET", "www.rottentomatoes.com/m/sleight");
//    cout << r_ptr->asio_response;
//
//}
//
void test_curl_authentication() {
    Task tsk("https://canvas.instructure.com/api/v1/courses/13960000000035155/students", nullptr,
             Authentication("access_token","xxx"));
    Downloader::Curl_Downloader cd;
    std::shared_ptr<Response> r_ptr = cd.get(tsk);
    cout << r_ptr->asio_response;

}

void test_curl_post() {
    // Let's have a post request task.
    Task tsk("https://httpbin.org/post", nullptr, Request_method::POST,
             Form {{"custname","Yanlin"}, {"custemail","duanyanl97@gmail.com"}});

    Downloader::Curl_Downloader cd;
    std::shared_ptr<Response> r_ptr = cd.post(tsk);
    cout << r_ptr->asio_response;

    // This would also work (sequence does not matter!):
    Task tsk2("https://httpbin.org/post", nullptr, Form {{"custname","Alan"}, {"custemail","hello@world.com"}},
             Request_method::POST);

    cout << cd.post(tsk2)->asio_response;

}

int main(int argc, char* argv[])
{
//    test_request_basic();
//    test_curl_request_basic();
//    test_curl_download();
//    test_curl_download_https();
//    test_curl_authentication();
//    test_curl_post();
    test_curl_auth2();
//    test_session_basic();
    return 0;
}


