//
// Created by green on 25/10/17.
//
#include <exception>
#include "inc/Data/NodeContainer.h"

using namespace node;
using namespace std;
using namespace web;

NodeContainer::NodeContainer()
{
  try {
    m_Nodes.reserve(100);
  }
  catch (exception& e)
    {
      cerr << "Standard exception: " << e.what() << endl;
    }
}

void NodeContainer::PrintNodes()
{
  boost::mutex n_mutex;

  boost::mutex::scoped_lock lock(n_mutex);

  try {
    for (auto const &i : m_Nodes)
      {
	std::cerr << i->getSSID();
	std::cerr << i->getRSSI();
	std::cerr << i->getMAC();
	std::cerr << "\n";
      }
  }
  catch (exception& e)
    {
      cerr << "Standard exception: " << e.what() << endl;
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
    // cout << yourJson.serialize();
    return yourJson;
}

void NodeContainer::ClearNodes()
{
  boost::mutex n_mutex;

  boost::mutex::scoped_lock lock(n_mutex);

  try { 
    if (m_Nodes.size() > 0)
      {
	m_Nodes.clear();
      }
  }
  catch (exception& e)
    {
      cerr << "Standard exception: " << e.what() << endl;
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

//
// nodes is container of recently scanned nodes with current RSS values
//
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
	  if (!strcasecmp(x->getMAC().c_str(), i->getMAC().c_str())  )
            {
	      // i is scanned node with current RSS value but no coordinates
	      // x is read node with known coordinates
	      //
	      x->Update(i);
	      x->setRecentlyUpdated(true);
            }
        }
    }
}


