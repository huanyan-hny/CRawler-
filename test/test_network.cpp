#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include "../Network.h"

using boost::asio::ip::tcp;
using namespace Crawler;


void test_request_basic() {

    Request request("get","www.imdb.com/title/tt0796366/");
    std::shared_ptr<Response> r_ptr = request.get();
//    cout << r_ptr->asio_response;
}


void test_session_basic() {
    Crawler::Session s {};
    std::shared_ptr<Response> r_ptr = s.request("GET", "www.imdb.com/title/tt0796366/");
    cout << r_ptr->header;
//    string a = r_ptr->header.find("Set-Cookie");
//    cout << r_ptr->asio_response;

}

int main(int argc, char* argv[])
{
    test_request_basic();
    test_session_basic();

    return 0;
}


