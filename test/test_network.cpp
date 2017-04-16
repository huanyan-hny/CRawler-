#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include "../Network.h"

using boost::asio::ip::tcp;
using namespace Crawler;

int main(int argc, char* argv[])
{
        Request request;
        request.request_method = Request_method::GET;
        request.url = "www.boost.org";
        request.resource = "/LICENSE_1_0.txt";
        std::unique_ptr<Response> r_ptr = request.get();
        cout << r_ptr->asio_response;
        return 0;

}
