#include "inc/stdafx.h"
#include "inc/Modules/RestModule.h"
#ifdef __arm__
#include "inc/Modules/WifiScanModule.h"
#else
#include "inc/Modules/WifiScanModulex86Test.h"
#endif

#include <future>

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

std::shared_ptr<RestModule> p_RestMod;
std::shared_ptr<FileNodeReaderModule> p_HttpGetMod;
std::shared_ptr<LocationModule> p_LocMod;
#ifdef __arm__
std::shared_ptr<WifiScanModule> p_ScanMod;
#else
std::shared_ptr<WifiScanModulex86Test> p_ScanMod;
#endif
vector<shared_ptr<IModule>> moduleList;
std::shared_ptr<node::NodeContainer> targetNodes;

void on_initialize(const string_t &address)
{
    // Build our listener's URI from the configured address and the hard-coded path "MyServer/Action"

    uri_builder uri(address);
    uri.append_path(U("Triangulation"));
    auto addr = uri.to_uri().to_string();
    std::cout << addr << std::endl;

    p_HttpGetMod = std::make_shared<FileNodeReaderModule>("Settings.json",targetNodes);
    p_LocMod = std::make_shared<LocationModule>(targetNodes);
    #ifdef __arm__
    p_ScanMod = std::make_shared<WifiScanModule>();
    #else
    p_ScanMod = std::make_shared<WifiScanModulex86Test>();
    #endif

    p_RestMod = std::make_shared<RestModule>(addr, "http://marconi.sdsu.edu:8080/GeoLocation/resources/ap", p_LocMod,
                                             p_ScanMod, p_HttpGetMod);

    moduleList.push_back(p_HttpGetMod);
    moduleList.push_back(p_LocMod);
    moduleList.push_back(p_ScanMod);
    moduleList.push_back(p_RestMod);

    for (auto &i : moduleList)
    {
        i->initialize();
    }


    while (true)
    {
        targetNodes->UpdateNodes(p_ScanMod->getScannedNodes());
        boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
    }

}

void on_shutdown()
{
    for (auto &i : moduleList)
    {
        i->deInitialize();
    }
    return;
}

int main(int argc, wchar_t *argv[])
{
    utility::string_t port = U("34568");
    if (argc == 2)
    {
        std::wstring ws(argv[1]);
        std::string port(ws.begin(), ws.end());

    }

    utility::string_t address = U("http://0.0.0.0:");
    address.append(port);

    on_initialize(address);
    std::promise<void> p;
    p.get_future().wait();

    on_shutdown();
    return 0;
}
