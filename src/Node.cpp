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


Node::Node(std::string Name, int Rssi, std::string Mac):m_name(Name),m_rssi(Rssi),m_mac(Mac){
}

Node::Node(web::json::value node) {
    try {
        m_name = node["ssid"].as_string();
        cout << node["ssid"];
    }catch(std::exception e){
        printf("Error:: Cannot parse SSID from JSON. Please check Syntax");
        std::exception ParseError;
        throw(ParseError);
    }
    try{
        m_rssi = node["rssi"].as_double();
        cout << node["rssi"];
    }catch(std::exception e){
        printf("Error:: Cannot parse RSSI from JSON. Substituing with 0");
        m_rssi=0;
    }
    try{
        m_mac = node["mac"].as_string();
        cout << node["mac"];
    }catch(std::exception e){
        printf("Error:: Cannot parse mac from JSON. Please check Syntax");
        std::exception ParseError;
        throw(ParseError);
    }



}

web::json::value Node::ToJson(){
using namespace web;
    //value::parse(U("{ \"ssid\" : \""+m_name+"\", \"m_rssi\" : "+std::to_string(m_rssi)+" }"));

    json::value response = json::value::object();
    response["ssid"] = json::value::string(m_name);
    response["rssi"] = json::value::number(m_rssi);
    response["mac"] = json::value::string(m_mac);
    return response;
}


 int Node::getRSSI() const{
    return m_rssi;
}

std::string Node::getSSID() {
    return m_name;
}

std::string Node::getMAC(){
    return m_mac;
}
void Node::Update(std::shared_ptr<INode> Node){
    m_rssi=Node->getRSSI();
    m_name=Node->getSSID();
    m_name=Node->getMAC();
}