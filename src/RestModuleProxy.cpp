#include "inc/stdafx.h"
#include "inc/messagetypes.h"
#include "inc/RestModuleProxy.h"


#include "boost/date_time/posix_time/posix_time.hpp"
using namespace std;
using namespace web; 
using namespace utility;
using namespace http;
using namespace web::http::experimental::listener;

web::json::value yourJson;
std::string jsonstr;

MyServer::MyServer(utility::string_t url) : m_listener(url)
{
    yourJson=json::value();
    m_listener.support(methods::GET, std::bind(&MyServer::handle_get, this, std::placeholders::_1));
    m_listener.support(methods::PUT, std::bind(&MyServer::handle_put, this, std::placeholders::_1));
    m_listener.support(methods::POST, std::bind(&MyServer::handle_post, this, std::placeholders::_1));
    m_listener.support(methods::DEL, std::bind(&MyServer::handle_delete, this, std::placeholders::_1));


}

void MyServer::handle_get(http_request message) //Replace this with Json Data received via PUT (INCLUDE TIMESTAMP)
{
/*
    yourJson[U("System")][U("Target")] = web::json::value(TargetNodes->ToJson());
    yourJson[U("System")][U("Scan")]  = web::json::value(ScannedNodes->ToJson());
    yourJson[U("System")][U("Location")]  = web::json::value(LocationModule->ToJson());
    yourJson[U("System")][U("ScanTime")]  = web::json::value(scanTime);
*/

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


    message.reply(status_codes::OK)
            .wait();
    message.extract_json().then([=](json::value request){
        boost::posix_time::ptime timeLocal = boost::posix_time::second_clock::local_time();
        std::stringstream localTimeString;
        localTimeString<< timeLocal << std::endl;
            yourJson = json::value(request);
            yourJson["POST_Time"] = json::value::string(localTimeString.str());

    }).wait();

    message.reply(status_codes::OK);

};

void MyServer::handle_delete(http_request message)
{
	ucout << message.to_string() << endl;
	message.reply(status_codes::OK);
}

void MyServer::handle_put(http_request message) //Replace this with code for parsing "PUT" Beaglebone data.
{
    /*printf("Handling PUT");
	ucout << message.to_string() << endl;

	auto paths = uri::split_path(uri::decode(message.relative_uri().path()));
	auto query = uri::split_query(uri::decode(message.relative_uri().query()));
	auto queryItr = query.find(U("request"));
	utility::string_t request = queryItr->second;
	ucout << request << endl;

    //yourJson=message.extract_json().get();
    pplx::task<json::value> body_json = message.extract_json();
    yourJson=web::json::value(body_json.get());
    cout<<yourJson.serialize();*/

    /*
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
	}*/

	message.reply(status_codes::OK,yourJson.serialize());
};

