//
// Created by green on 25/10/17.
//

#include "inc/Node.h"
#include "../../build_panther/usr/local/include/cpprest/json.h"
#include <map>
#include <set>
#include <cmath>
using namespace std;
Node::Node(){

}

Node::Node(std::string Name, std::string Mac, int SigStrength, int Rssi){

}
double Node::CalculateDistance(){

    printf("Calculating range base on RSSI of:%d",m_Rssi);
    double Power=(-(static_cast<double>(m_Rssi) - (-60.0))/(10.0*3.0));

    printf("Calculating Distance: %f",pow(10.0,Power));

    return pow(10.0,Power);
}

web::json::value Node::ToJson(){
using namespace web;
    //value::parse(U("{ \"ssid\" : \""+m_name+"\", \"m_rssi\" : "+std::to_string(m_Rssi)+" }"));

    auto response = json::value::object();

    response["SSID"] = json::value::string(m_name);
    response["RSSI"] = json::value::number(m_Rssi);
    response["Distance"] = json::value::number(CalculateDistance());
    response["Channel"] = json::value::number(m_channel);
    auto aValue = response.at(U("RSSI"));
    cout << aValue;
    return response;
}