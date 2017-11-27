//
// Created by green on 25/10/17.
//

#include "inc/Node.h"
#include "../../build_panther/usr/local/include/cpprest/json.h"
#include <map>
#include <set>
#include <cmath>
#include <utility>
using namespace std;
Node::Node(){

}

Node::Node(web::json::value node){
    try {
        const std::string &s = "SSID";
        cout << node["SSID"];
        m_name = node["SSID"].as_string();
        cout << node["XCoord"];
        y_coord = node["XCoord"].as_double();
        cout << node["YCoord"];
        x_coord = node["YCoord"].as_double();
        cout << node["RSSICalib"];
        m_RssiCalib = node["RSSICalib"].as_double();
    }catch(std::exception e){
        printf("Error:: Cannot parse node from settings file.");
    }


}

Node::Node(std::string Name, std::string Mac, int SigStrength, int Rssi){


}
double Node::CalculateDistance(){

    printf("Calculating range base on RSSI of:%d",m_Rssi);
    try {
        double Power = (-(static_cast<double>(m_Rssi) - (m_RssiCalib)) / (10.0 * 3.0));

        printf("Calculating Distance: %f", pow(10.0, Power));

        return pow(10.0, Power);
    }catch(std::exception X){
        printf("Error:: No Calibration Data Available");
        return 0;
    }
}

web::json::value Node::ToJson(){
using namespace web;
    //value::parse(U("{ \"ssid\" : \""+m_name+"\", \"m_rssi\" : "+std::to_string(m_Rssi)+" }"));

    auto response = json::value::object();

    response["SSID"] = json::value::string(m_name);
    response["RSSI"] = json::value::number(m_Rssi);
    response["Distance"] = json::value::number(CalculateDistance());
    response["XCoord"] = json::value::number(x_coord);
    response["YCoord"] = json::value::number(y_coord);
    response["RSSICalib"] = json::value::number(m_RssiCalib);
    response["Channel"] = json::value::number(m_channel);
    auto aValue = response.at(U("RSSI"));
    cout << aValue;
    return response;
}