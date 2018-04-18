#include "inc/stdafx.h"
#include "inc/messagetypes.h"
#include "inc/RestModule.h"
#include "inc/Node_Container.h"
#include "inc/FileIO.h"
#include "inc/WifiScanModule.h"
#include "inc/locationModule.h"
#include <cpprest/http_client.h>
#include "inc/GETNodes.h"
using namespace std;
using namespace web;
using namespace utility;
using namespace http;
using namespace web::http::experimental::listener;
std::shared_ptr<node::Node_Container> ScannedNodes;
std::shared_ptr<node::Node_Container> TargetNodes;
std::shared_ptr<WifiScanModule> ScanModule;
std::shared_ptr<locationModule> LocationModule;
double scanTime;

MyServer::MyServer(utility::string_t url) : m_listener(url)
{
    TargetNodes =  std::make_shared<node::Node_Container>();

    GETNodes GETNodes("http://marconi.sdsu.edu:8080/GeoLocation/resources/ap");
    TargetNodes=GETNodes.get();
    m_listener.support(methods::GET, std::bind(&MyServer::handle_get, this, std::placeholders::_1));
    m_listener.support(methods::PUT, std::bind(&MyServer::handle_put, this, std::placeholders::_1));
    m_listener.support(methods::POST, std::bind(&MyServer::handle_post, this, std::placeholders::_1));
    m_listener.support(methods::DEL, std::bind(&MyServer::handle_delete, this, std::placeholders::_1));

    ScannedNodes = std::make_shared<node::Node_Container>();
    LocationModule = std::make_shared<locationModule>();
    ScanModule = std::make_shared<WifiScanModule>(ScannedNodes,TargetNodes,LocationModule,scanTime);

    boost::thread t(boost::bind(&WifiScanModule::Scan,ScanModule));

    boost::thread t2(boost::bind(&MyServer::PostData,this));


}

void MyServer::handle_get(http_request message)
{
    std::cout << message.relative_uri().to_string() << std::endl;

    ucout <<  message.to_string() << endl;
    web::json::value yourJson;

    yourJson[U("System")][U("Target")] = web::json::value(TargetNodes->ToJson());
    yourJson[U("System")][U("Scan")]  = web::json::value(ScannedNodes->ToJson());
    yourJson[U("System")][U("Location")]  = web::json::value(LocationModule->ToJson());
    yourJson[U("System")][U("ScanTime")]  = web::json::value(scanTime);

    auto query_string = message.absolute_uri().query();
    auto query_map = uri::split_query(query_string);
    auto it = query_map.find(U("callback"));
    if (it != query_map.end())
    {
        // Query uses JSONP
        auto callback = it->second;
        std::stringstream ss;
        ss << callback << "(" << yourJson.serialize() << ")";

        message.reply(status_codes::OK, ss.str());

    }else {
        cout << "Got To Here";
        message.reply(status_codes::OK, yourJson);

    }


};

void MyServer::handle_post(http_request message)
{
    ucout <<  message.to_string() << endl;
	message.reply(status_codes::OK);
};

void MyServer::handle_delete(http_request message)
{
	ucout << message.to_string() << endl;
	message.reply(status_codes::OK);
}

void MyServer::handle_put(http_request message)
{
	message.reply(status_codes::OK);
};

void MyServer::PostData(){
    while(true){
        Post();
        boost::this_thread::sleep_for(boost::chrono::milliseconds(2000));
    }
}

pplx::task<int> MyServer::Post()
{
    return pplx::create_task([]
                             {
                                 web::json::value yourJson;
                                 yourJson=LocationModule->BasicJson();
                                 yourJson[U("ID")] = web::json::value("User0");
                                 yourJson[U("Site")] = web::json::value::number(0);


                                 http_client client("http://marconi.sdsu.edu:8080/GeoLocation/resources/ap");

                                 return client.request(methods::POST,"",
                                                       yourJson.serialize(), "application/json");
                             }).then([](http_response response)
                                     {
                                         if(response.status_code() == status_codes::OK)
                                         {
                                             return 1;
                                         }

                                         return 0;
                                     });
}


