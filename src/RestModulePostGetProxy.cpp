#include "inc/stdafx.h"
#include "inc/Modules/RestModulePostGetProxy.h"


#include "boost/date_time/posix_time/posix_time.hpp"
using namespace std;
using namespace web; 
using namespace utility;
using namespace http;
using namespace web::http::experimental::listener;

web::json::value yourJson;
std::string jsonstr;

RestModule_Post_Get_Proxy::RestModule_Post_Get_Proxy(utility::string_t url) : m_listener(url)
{
    yourJson=json::value();
    m_listener.support(methods::GET, std::bind(&RestModule_Post_Get_Proxy::handle_get, this, std::placeholders::_1));
    m_listener.support(methods::PUT, std::bind(&RestModule_Post_Get_Proxy::handle_put, this, std::placeholders::_1));
    m_listener.support(methods::POST, std::bind(&RestModule_Post_Get_Proxy::handle_post, this, std::placeholders::_1));
    m_listener.support(methods::DEL, std::bind(&RestModule_Post_Get_Proxy::handle_delete, this, std::placeholders::_1));


}

void RestModule_Post_Get_Proxy::handle_get(http_request message) //Replace this with Json Data received via PUT (INCLUDE TIMESTAMP)
{

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

void RestModule_Post_Get_Proxy::handle_post(http_request message)
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

void RestModule_Post_Get_Proxy::handle_delete(http_request message)
{
	ucout << message.to_string() << endl;
	message.reply(status_codes::OK);
}

void RestModule_Post_Get_Proxy::handle_put(http_request message) //Replace this with code for parsing "PUT" Beaglebone data.
{

    ucout << message.to_string() << endl;
	message.reply(status_codes::OK);
};

