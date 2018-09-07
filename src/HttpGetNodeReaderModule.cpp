//
// Created by green on 15/04/18.
//

#include "inc/Modules/HttpGetNodeReaderModule.h"

HttpGetNodeReaderModule::HttpGetNodeReaderModule(std::string url,std::shared_ptr<node::NodeContainer>& nodeContainer) : m_Url(url), m_NodeContainer(nodeContainer)
{
}


web::json::value HttpGetNodeReaderModule::GetRequest()
{

    boost::mutex::scoped_lock lock(g_i_mutex);
    using namespace web::http::client;
    using namespace web::http;
    http_client client(U(m_Url));

    return client.request(methods::GET, "").get().extract_json().get();
}

std::shared_ptr<node::NodeContainer> HttpGetNodeReaderModule::readNodes()
{
    std::shared_ptr<node::NodeContainer> p_NodeList;
    p_NodeList = std::make_shared<node::NodeContainer>();
    web::json::value v = GetRequest()["ap"];

    for (int i = 0; i < v.as_array().size(); i++)
    {
        try
        {
	  // Initially, nodes will not have an RSSI defined in the JSON obtained from the database
	  //
	  std::shared_ptr<TargetNode> newNode(std::make_shared<TargetNode>(v.as_array().at(i)));
	  p_NodeList->AddNode(newNode);
        } catch (std::exception e)
        {
            std::cout << "Error Node: " << i << " Invalid. Discarding..." << std::endl;
        }
        //std::cout << "AFTER PARSING " << i <<": " << v.as_array()[i].serialize();
    };
    return p_NodeList;
}


void HttpGetNodeReaderModule::initialize()
{
    try
    {
        m_NodeContainer= readNodes();
        m_isRunning = true;
    }
    catch(std::exception e)
    {
        std::cout<<"Couldn't initialize HTTPGetNodeReaderModule" <<std::endl;
    }
}

void HttpGetNodeReaderModule::deInitialize()
{
    m_isRunning = false;
}

bool HttpGetNodeReaderModule::isRunning()
{

    return m_isRunning;
}
