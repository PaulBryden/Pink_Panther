
#ifndef FOO_REST_MODULE_H
#define FOO_REST_MODULE_H

#include <cpprest/http_client.h>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include "../Data/NodeContainer.h"
#include "FileNodeReaderModule.h"
#include "WifiScanModule.h"
#include "LocationModule.h"
#include <cpprest/http_client.h>
#include "HttpGetNodeReaderModule.h"

using namespace std;
using namespace web;
using namespace utility;
using namespace http;
using namespace web::http::experimental::listener;
using namespace std;
using namespace web;
using namespace utility;
using namespace http;
using namespace web::http::experimental::listener;
using namespace web::http::client;

class RestModule : public IModule
{
public:
    RestModule();

    ~RestModule();

    RestModule(utility::string_t hostUrl, utility::string_t postURL,
               std::shared_ptr<ILocationModule> locModulePtr, std::shared_ptr<IScanModule> scanModulePtr,
               std::shared_ptr<INodeReaderModule> readerModulePtr);

    void initialize();

    void deInitialize();

    bool isRunning();


private:
    bool m_isRunning;

    void handle_get(http_request message);

    void handle_put(http_request message);

    void handle_post(http_request message);

    void handle_delete(http_request message);

    void PostData();

    boost::mutex g_i_mutex;

    std::shared_ptr<node::NodeContainer> m_ScannedNodes;
    std::shared_ptr<node::NodeContainer> TargetNodes;
    std::shared_ptr<IScanModule> m_ScanModule;
    std::shared_ptr<ILocationModule> m_LocationModule;
    std::shared_ptr<INodeReaderModule> m_ReaderModule;
    http_listener m_listener;
    std::shared_ptr<boost::thread> m_ScannerPtr;
    std::string m_PostURL;
};

#endif