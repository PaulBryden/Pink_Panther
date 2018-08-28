//
// Created by green on 25/10/17.
//

#include "inc/Data/Node.h"

using namespace std;


Node::Node(std::string Name, int Rssi, std::string Mac) : m_name(Name), m_rssi(Rssi), m_mac(Mac),
                                                          m_Recently_Updated(true)
{
}

Node::Node(web::json::value node) : m_Recently_Updated(true)
{
    try
    {
        m_name = node["ssid"].as_string();
    } catch (std::exception e)
    {
        printf("Error:: Cannot parse SSID from JSON. Please check Syntax\n");
        std::exception ParseError;
        throw (ParseError);
    }
    try
    {
      m_rssi = node["rssi"].as_double();
    } catch (std::exception e)
    {
      // Initially, nodes will not have an RSSI defined in the JSON obtained from the database
      //
      // printf("Error:: Cannot parse RSSI from JSON. Substituing with 0\n");
      m_rssi = 0;
    }
    try
    {
        m_mac = node["mac"].as_string();
    } catch (std::exception e)
    {
        printf("Error:: Cannot parse mac from JSON. Please check Syntax");
        std::exception ParseError;
        throw (ParseError);
    }
}

web::json::value Node::ToJson()
{
    using namespace web;
    //value::parse(U("{ \"ssid\" : \""+m_name+"\", \"m_rssi\" : "+std::to_string(m_rssi)+" }"));

    json::value response = json::value::object();
    response["ssid"] = json::value::string(m_name);
    response["rssi"] = json::value::number(m_rssi);
    response["mac"] = json::value::string(m_mac);
    return response;
}


int Node::getRSSI()
{
    return m_rssi;
}

std::string Node::getSSID()
{
    return m_name;
}

std::string Node::getMAC()
{
    return m_mac;
}

void Node::Update(std::shared_ptr<INode> Node)
{
    m_rssi = Node->getRSSI();
    m_name = Node->getSSID();
    //    m_name = Node->getMAC();
    m_mac = Node->getMAC();
}

void Node::setRecentlyUpdated(bool status)
{
    m_Recently_Updated = status;
}

bool Node::getRecentlyUpdated()
{
    return m_Recently_Updated;
}
