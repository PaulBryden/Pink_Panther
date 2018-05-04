//
// Created by green on 25/10/17.
//

#include "inc/Data/NodeContainer.h"

using namespace node;
using namespace std;
using namespace web;

NodeContainer::NodeContainer()
{

}


void NodeContainer::PrintNodes()
{
    for (auto const &i : m_Nodes)
    {
        std::cout << i->getSSID();
        std::cout << i->getRSSI();
        std::cout << i->getMAC();
        std::cout << "/n";


    }
}

web::json::value NodeContainer::ToJson()
{
    boost::mutex::scoped_lock lock(g_i_mutex);
    web::json::value yourJson = web::json::value::object();
    yourJson[U("Nodes")] = web::json::value::array(m_Nodes.size());
    int i = 0;
    for (i = 0; i < m_Nodes.size(); i++)
    {

        yourJson[U("Nodes")].as_array()[i] = web::json::value(m_Nodes[i]->ToJson());
    }
    cout << yourJson.serialize();
    return yourJson;
}

void NodeContainer::ClearNodes()
{
    boost::mutex::scoped_lock lock(g_i_mutex);
    if (m_Nodes.size() > 0)
    {
        m_Nodes.clear();
    }
}

std::vector<std::shared_ptr<INode>> &NodeContainer::GetNodes()
{
    return m_Nodes;
}

void NodeContainer::AddNode(std::shared_ptr<INode> node)
{
    boost::mutex::scoped_lock lock(g_i_mutex);
    m_Nodes.push_back(node);
}

void NodeContainer::UpdateNodes(std::shared_ptr<NodeContainer> nodes)
{
    boost::mutex::scoped_lock lock(g_i_mutex);

    for (auto &x : m_Nodes)
    {

        x->setRecentlyUpdated(false);
    }
    for (auto &i : nodes->GetNodes())
    {
        for (auto &x : m_Nodes)
        {
            if (x->getMAC() == i->getMAC())
            {
                x->Update(i);
                x->setRecentlyUpdated(true);
            }
        }
    }

}


