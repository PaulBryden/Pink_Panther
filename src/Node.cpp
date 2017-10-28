//
// Created by green on 25/10/17.
//

#include "inc/Node.h"

Node::Node(){

}

Node::Node(std::string Name, std::string Mac, int SigStrength, int Rssi){

}

web::json::value Node::ToJson(){

    using namespace web;
    json::value obj;
    obj[L"ssid"] = json::value::string(m_name);
    obj[L"m_rssi"] = json::value::number(m_Rssi);

    return obj;
}