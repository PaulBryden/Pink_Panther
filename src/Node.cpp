//
// Created by green on 25/10/17.
//

#include "inc/Node.h"
#include "../../build_panther/usr/local/include/cpprest/json.h"
#include <map>
#include <set>

using namespace std;
Node::Node(){

}

Node::Node(std::string Name, std::string Mac, int SigStrength, int Rssi){

}

web::json::value Node::ToJson(){
using namespace web;
    //value::parse(U("{ \"ssid\" : \""+m_name+"\", \"m_rssi\" : "+std::to_string(m_Rssi)+" }"));

    auto response = json::value::object();

    response["SSID"] = json::value::string(m_name);
    response["RSSI"] = json::value::number(m_Rssi);
    auto aValue = response.at(U("RSSI"));
    cout << aValue;
    return response;
}