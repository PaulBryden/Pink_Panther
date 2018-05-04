#include "inc/stdafx.h"
#include "inc/Modules/RestModule.h"
#include "inc/Exceptions/ModuleInitializationException.h"
double scanTime;

RestModule::RestModule(utility::string_t hostUrl, utility::string_t postURL,
                       std::shared_ptr<ILocationModule>locModulePtr, std::shared_ptr<IScanModule> scanModulePtr, std::shared_ptr<INodeReaderModule> readerModulePtr) : m_listener(hostUrl),
                                                                              m_LocationModule(locModulePtr),
                                                                              m_ScanModule(scanModulePtr),
                                                                                m_ReaderModule(readerModulePtr){




}

void RestModule::initialize() {
    try{
        TargetNodes = std::make_shared<node::NodeContainer>();

        //HttpGetNodeReaderModule readerModulePtr("http://marconi.sdsu.edu:8080/GeoLocation/resources/ap");
        m_LocationModule->initialize();
        m_ScanModule->initialize();
        m_ReaderModule->initialize();

        TargetNodes = m_ReaderModule->readNodes();
        m_listener.support(methods::GET, std::bind(&RestModule::handle_get, this, std::placeholders::_1));
        m_listener.support(methods::PUT, std::bind(&RestModule::handle_put, this, std::placeholders::_1));
        m_listener.support(methods::POST, std::bind(&RestModule::handle_post, this, std::placeholders::_1));
        m_listener.support(methods::DEL, std::bind(&RestModule::handle_delete, this, std::placeholders::_1));

    }catch(std::exception e){
        ModuleInitializationException ex();
        throw(ex);
    }

    boost::thread t(boost::bind(&RestModule::PostData, this));
}


void RestModule::handle_get(http_request message) {
    std::cout << message.relative_uri().to_string() << std::endl;

    ucout << message.to_string() << endl;
    web::json::value yourJson;

    yourJson[U("System")][U("Target")] = web::json::value(TargetNodes->ToJson());
    yourJson[U("System")][U("Scan")] = web::json::value(m_ScannedNodes->ToJson());
    yourJson[U("System")][U("Location")] = web::json::value(m_LocationModule->GetJson());
    yourJson[U("System")][U("ScanTime")] = web::json::value(scanTime);

    auto query_string = message.absolute_uri().query();
    auto query_map = uri::split_query(query_string);
    auto it = query_map.find(U("callback"));
    if (it != query_map.end()) {
        // Query uses JSONP
        auto callback = it->second;
        std::stringstream ss;
        ss << callback << "(" << yourJson.serialize() << ")";

        message.reply(status_codes::OK, ss.str());

    } else {
        cout << "Got To Here";
        message.reply(status_codes::OK, yourJson);

    }


};

void RestModule::handle_post(http_request message) {
    ucout << message.to_string() << endl;
    message.reply(status_codes::OK);
};

void RestModule::handle_delete(http_request message) {
    ucout << message.to_string() << endl;
    message.reply(status_codes::OK);
}

void RestModule::handle_put(http_request message) {
    message.reply(status_codes::OK);
};

void RestModule::PostData() {
    while (true) {

        try {
            // Post();
            web::json::value yourJson;
            yourJson = m_LocationModule->BasicJson();
            yourJson[U("ID")] = web::json::value("User0");
            yourJson[U("Site")] = web::json::value::number(1);

            http_client client("http://marconi.sdsu.edu:9999/RestModule/Action/");
            client.request(methods::POST, "",
                           yourJson.serialize(), "application/json").get();

        } catch (std::exception e) {

        }
        catch (const boost::system::system_error &e) {
        }

        boost::this_thread::sleep_for(boost::chrono::milliseconds(500));

    }
}