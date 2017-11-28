#include "stdafx.h"
#include "messagetypes.h"
#include "MyServer.h"
#include "inc/Node_Container.h"
#include "inc/FileIO.h"
#include "inc/WifiScanModule.h"
using namespace std;
using namespace web; 
using namespace utility;
using namespace http;
using namespace web::http::experimental::listener;
std::shared_ptr<node::Node_Container> ScannedNodes;
std::shared_ptr<node::Node_Container> TargetNodes;
std::shared_ptr<WifiScanModule> ScanModule;

MyServer::MyServer(utility::string_t url) : m_listener(url)
{
    TargetNodes = new node::Node_Container();
    FileIO::FileIO newFileIO(TargetNodes);
    newFileIO.ReadIn();
    m_listener.support(methods::GET, std::bind(&MyServer::handle_get, this, std::placeholders::_1));
    m_listener.support(methods::PUT, std::bind(&MyServer::handle_put, this, std::placeholders::_1));
    m_listener.support(methods::POST, std::bind(&MyServer::handle_post, this, std::placeholders::_1));
    m_listener.support(methods::DEL, std::bind(&MyServer::handle_delete, this, std::placeholders::_1));

    ScannedNodes = new node::Node_Container();
    ScanModule = new WifiScanModule(ScannedNodes,TargetNodes);

    boost::thread t{ScanModule->Scan()};

}

void MyServer::handle_get(http_request message)
{
    ucout <<  message.to_string() << endl;
    web::json::value yourJson;
    yourJson[U("System")] = web::json::value::array(2);
    yourJson[U("System")].as_array()[0] = web::json::value(TargetNodes->ToJson());
    yourJson[U("System")].as_array()[1] = web::json::value(ScannedNodes->ToJson());

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
	ucout << message.to_string() << endl;

	auto paths = uri::split_path(uri::decode(message.relative_uri().path()));
	auto query = uri::split_query(uri::decode(message.relative_uri().query()));
	auto queryItr = query.find(U("request"));
	utility::string_t request = queryItr->second;
	ucout << request << endl;

	if (request == U("leave"))
	{
		Data data;
		data.job = U("Devs");
		People p1;
		p1.age = 10;
		p1.name = U("Franck");
		data.peoples.push_back(p1);
		People p2;
		p2.age = 20;
		p2.name = U("Joe");
		data.peoples.push_back(p2);

		utility::string_t response = data.AsJSON().serialize();
		ucout << response << endl;

		message.reply(status_codes::OK, data.AsJSON());
		return;
	}

	message.reply(status_codes::OK);
};
