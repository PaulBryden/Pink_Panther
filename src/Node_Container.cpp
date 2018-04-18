//
// Created by green on 25/10/17.
//

#include "inc/Node_Container.h"
#include "../../build_panther/usr/local/include/cpprest/json.h"
#include <iostream>

using namespace node;
using namespace std;
using namespace web;
Node_Container::Node_Container(){

}


void Node_Container::PrintNodes(){
    for (auto const& i : m_Nodes) {
        std::cout << i->getSSID();
        std::cout << i->getRSSI();
        std::cout << i->getMAC();
        std::cout << "/n";


    }
}

web::json::value Node_Container::ToJson() {
    boost::mutex::scoped_lock lock(g_i_mutex);
    web::json::value yourJson = web::json::value::object();
    yourJson[U("Nodes")] = web::json::value::array(m_Nodes.size());
    int i = 0;
    for (i = 0; i < m_Nodes.size(); i++) {

         yourJson[U("Nodes")].as_array()[i] = web::json::value(m_Nodes[i]->ToJson());
    }
    cout << yourJson.serialize();
    return yourJson;
}
void Node_Container::ClearNodes(){
    boost::mutex::scoped_lock lock(g_i_mutex);
    if(m_Nodes.size()>0) {
        m_Nodes.clear();
    }
}

std::vector<std::shared_ptr<INode>>& Node_Container::GetNodes(){
    boost::mutex::scoped_lock lock(g_i_mutex);
    return m_Nodes;
}

void Node_Container::AddNode(std::shared_ptr<INode> node){
    boost::mutex::scoped_lock lock(g_i_mutex);
    m_Nodes.push_back(node);
}

void Node_Container::UpdateNodes(std::shared_ptr<Node_Container> nodes) {
    boost::mutex::scoped_lock lock(g_i_mutex);
     for(auto &i : nodes->GetNodes()){
         for (auto  &x : m_Nodes){
             if(x->getMAC()==i->getMAC()){
                 x->Update(i);
             }
         }
     }
}


