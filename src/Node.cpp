//
// Created by green on 25/10/17.
//

#include "inc/Node.h"
#include "../../build_panther/usr/local/include/cpprest/json.h"
#include <map>
#include <set>
#include <cmath>
#include <utility>
#include <memory>
using namespace std;


Node::Node(std::string Name, int RSSI, double Channel):m_name(Name),m_Rssi(RSSI),m_channel(Channel){
}

Node::Node(web::json::value node) {
    try {
        m_name = node["SSID"].as_string();
        cout << node["SSID"];
    }catch(std::exception e){
        printf("Error:: Cannot parse SSID from JSON. Please check Syntax");
        return;
    }
    try {
        m_channel = node["Channel"].as_double();
        cout << node["Channel"];
    }catch(std::exception e){
        printf("Error:: Cannot parse Channel from JSON. Please check Syntax");
        return;
    }
    try{
        m_Rssi = node["RSSI"].as_double();
        cout << node["RSSI"];
    }catch(std::exception e){
        printf("Error:: Cannot parse RSSI from JSON. Please check Syntax");
        return;
    }

}

web::json::value Node::ToJson(){
using namespace web;
    //value::parse(U("{ \"ssid\" : \""+m_name+"\", \"m_rssi\" : "+std::to_string(m_Rssi)+" }"));

    json::value response = json::value::object();
    response["SSID"] = json::value::string(m_name);
    response["RSSI"] = json::value::number(m_Rssi);
    response["Channel"] = json::value::number(m_channel);
    return response;
}

double Node::getChannel() {
    return m_channel;
}

int Node::getRSSI() {
    return m_Rssi;
}

std::string Node::getSSID() {
    return m_name;
}

void Node::Update(std::shared_ptr<INode> Node){
    m_channel=Node->getChannel();
    m_Rssi=Node->getRSSI();
    m_name=Node->getSSID();
}