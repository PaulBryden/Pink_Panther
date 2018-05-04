
#ifndef FOO_REST_MODULE_H
#define FOO_REST_MODULE_H

#include <cpprest/json.h>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include "Interfaces/IModule.h"

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

class RestModule_Post_Get_Proxy : public IModule
{
public:
	RestModule_Post_Get_Proxy();
	RestModule_Post_Get_Proxy(utility::string_t url);

	void initialize();
	bool isRunning();
	pplx::task<void> open() { return m_listener.open(); }
	pplx::task<void> close() { return m_listener.close(); }

private:
	bool m_isRunning;

	void handle_get(http_request message);
	void handle_put(http_request message);
	void handle_post(http_request message);
	void handle_delete(http_request message);
	http_listener m_listener;
    boost::mutex g_i_mutex;
};

#endif