#include "inc/stdafx.h"
#include "inc/Modules/RestModule.h"
#include "inc/Exceptions/ModuleInitializationException.h"

double scanTime;

RestModule::RestModule(utility::string_t hostUrl, utility::string_t postURL,
                       std::shared_ptr<ILocationModule> locModulePtr, std::shared_ptr<IScanModule> scanModulePtr,
                       std::shared_ptr<INodeReaderModule> readerModulePtr) : m_listener(hostUrl),
                                                                             m_LocationModule(locModulePtr),
                                                                             m_ScanModule(scanModulePtr),
                                                                             m_ReaderModule(readerModulePtr),
                                                                             m_PostURL(postURL)
{
}

RestModule::~RestModule()
{
    deInitialize();
}

void RestModule::initialize()
{
    try
    {
        if (!m_isRunning)
        {
            m_TargetNodes = std::make_shared<node::NodeContainer>();
	    m_TargetNodes = m_ReaderModule->readNodes(); // GET request to retrieve AP coordinates from database
            m_listener.open();
            m_listener.support(methods::GET, std::bind(&RestModule::handle_get, this, std::placeholders::_1));
            m_listener.support(methods::PUT, std::bind(&RestModule::handle_put, this, std::placeholders::_1));
            m_listener.support(methods::POST, std::bind(&RestModule::handle_post, this, std::placeholders::_1));
            m_listener.support(methods::DEL, std::bind(&RestModule::handle_delete, this, std::placeholders::_1));
            m_isRunning = true;
            m_ScannerPtr = std::make_shared<boost::thread>(boost::bind(&RestModule::PostData, this));
        }
    }
    catch (std::exception e)
    {
        ModuleInitializationException ex;
        throw (ex);
    }
}

void RestModule::deInitialize()
{
    if (m_isRunning)
    {
        m_isRunning = false;
        boost::mutex::scoped_lock lock(g_i_mutex);
        m_ScannerPtr->interrupt();
        m_listener.close().wait();
    }
}

bool RestModule::isRunning()
{
    return m_isRunning;
}

void RestModule::handle_get(http_request message)
{
  // std::cout << message.relative_uri().to_string() << std::endl;

    ucout << message.to_string() << endl;
    web::json::value yourJson;
    yourJson[U("System")][U("Scan")] = web::json::value(m_ScanModule->getScannedNodes()->ToJson());
    yourJson[U("System")][U("Location")] = web::json::value(m_LocationModule->GetJson());
    yourJson[U("System")][U("ScanTime")] = web::json::value(m_ScanModule->getScanTime());
    yourJson[U("System")][U("TargetNodes")] = web::json::value(m_TargetNodes->ToJson());

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

    } else
    {
        cout << "Got To Here";
        message.reply(status_codes::OK, yourJson);

    }


};

void RestModule::handle_post(http_request message)
{
    ucout << message.to_string() << endl;
    message.reply(status_codes::OK);
};

void RestModule::handle_delete(http_request message)
{
    ucout << message.to_string() << endl;
    message.reply(status_codes::OK);
}

void RestModule::handle_put(http_request message)
{
    message.reply(status_codes::OK);
};

void RestModule::PostData()
{
    while (m_isRunning)
    {
        boost::mutex::scoped_lock lock(g_i_mutex);
	try
        {
            web::json::value yourJson;
            yourJson = static_pointer_cast<LocationModule>(m_LocationModule)->GetJson();
	    // yourJson = static_pointer_cast<LocationModule>(m_LocationModule)->BasicJson();
            yourJson[U("ID")] = web::json::value("User0");
            yourJson[U("Site")] = web::json::value::number(1);
            http_client client(m_PostURL);
	    // cerr << "RestModule::PostData: " << yourJson.serialize() << endl;
            client.request(methods::POST, "",
                           yourJson.serialize(), "application/json").get();
        }
        catch (std::exception e)
        {
	  // std::cerr << "RestModule::PostData: " << e.what();
        }
        boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
    }
}
