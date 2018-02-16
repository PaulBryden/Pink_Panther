#pragma once
#include <cpprest/http_client.h>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>

using namespace std;
using namespace web;
using namespace utility;
using namespace http;
using namespace web::http::experimental::listener;
using namespace web::http::client;

class MyServer
{
public:
	MyServer() {}
	MyServer(utility::string_t url);

	pplx::task<void> open() { return m_listener.open(); }
	pplx::task<void> close() { return m_listener.close(); }

private:

	void handle_get(http_request message);
	void handle_put(http_request message);
	void handle_post(http_request message);
	void handle_delete(http_request message);
	void PostData();
	pplx::task<int> Post();

	boost::mutex g_i_mutex;
	http_listener m_listener;

};

