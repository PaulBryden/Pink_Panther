#include "inc/stdafx.h"
#include "inc/RestModule.h"

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

std::unique_ptr<MyServer> g_http;

void on_initialize(const string_t& address)
{
    // Build our listener's URI from the configured address and the hard-coded path "MyServer/Action"

    uri_builder uri(address);
    uri.append_path(U("MyServer/Action/"));

    auto addr = uri.to_uri().to_string();
    g_http = std::unique_ptr<MyServer>(new MyServer(addr));
    g_http->open().wait();

    ucout << utility::string_t(U("Listening for requests at: ")) << addr << std::endl;

    return;
}

void on_shutdown()
{
    g_http->close().wait();
    return;
}

int main(int argc, wchar_t *argv[])
{
    utility::string_t port = U("34568");
    if(argc == 2)
    {
        std::wstring ws(argv[1]);
        std::string port(ws.begin(), ws.end());

    }

    utility::string_t address = U("http://127.0.0.1:");
    address.append(port);

    on_initialize(address);
    std::cout << "Press ENTER to exit." << std::endl;

    std::string line;
    std::getline(std::cin, line);

    on_shutdown();
    return 0;
}