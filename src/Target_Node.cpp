//
// Created by green on 27/11/17.
//

#include <cmath>
#include "inc/Target_Node.h"
#include "inc/Node.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <memory>

using namespace std;

void Target_Node::Update(std::shared_ptr<INode> Node){
    m_Node=Node;
    updateKalmanRSSI();
}

Target_Node::Target_Node(web::json::value node): m_Kalman_distance(0.000025, 0.17746,1),m_Kalman_rssi(0.0001, 2.36,1){
    m_Node=std::make_shared<Node>(node);

    try {
            m_XCoord = node["x"].as_double();
            cout << node["x"];
        }catch(std::exception e) {
            printf("Error:: Cannot parse XCoord from JSON. Please check Syntax");
            std::exception ParseError;
            throw(ParseError);
        }
    try {
        m_YCoord = node["y"].as_double();
        cout << node["y"];
    }catch(std::exception e) {
        printf("Error:: Cannot parse YCoord from JSON. Please check Syntax");
        std::exception ParseError;
        throw(ParseError);
    }
    try {
        m_ZCoord = node["z"].as_double();
        cout << node["z"];

    } catch(std::exception e) {
            printf("Error:: Cannot parse ZCoord from JSON. Please check Syntax");
        std::exception ParseError;
        throw(ParseError);
        }
    try{
        m_rssi_cal = node["cal"].as_integer();
        cout << node["cal"];
    }  catch(std::exception e) {
        printf("Error:: Cannot parse RSSICalib from JSON. Please check Syntax");
        std::exception ParseError;
        throw(ParseError);
    }



}

web::json::value Target_Node::ToJson(){
    //TO BE POPULATED
    using namespace web;
    json::value response = m_Node->ToJson();
    //value::parse(U("{ \"ssid\" : \""+m_name+"\", \"m_rssi\" : "+std::to_string(m_rssi)+" }"));

    response["XCoord"] = json::value::number(m_XCoord);
    response["YCoord"] = json::value::number(m_YCoord);
    response["ZCoord"] = json::value::number(m_ZCoord);
    response["RSSICalib"] = json::value::number(m_rssi_cal);
    response["Distance"] = json::value::number(CalculateDistance());
    response["RSSIFiltered"] = json::value::number(m_Kalman_RSSI_Val);
    //response["KalmanDistance"] = json::value::number(m_Kalman_Distance);
   // cout <<response.serialize();

    return response;


}

 int Target_Node::getRSSI() const{
    return m_Node->getRSSI();
}



std::string Target_Node::getSSID(){
    return m_Node->getSSID();
}

double Target_Node::getKalmanRSSI(){
    return m_Kalman_RSSI_Val;
}

void Target_Node::updateKalmanRSSI() {
    m_Kalman_RSSI_Val=m_Kalman_rssi.kalmanUpdate(this->getRSSI());
}

double Target_Node::CalculateDistance() {


    printf("Calculating range base on RSSI of:%d",m_Node->getRSSI());
    try {
        double Power = (-(static_cast<double>(m_Kalman_RSSI_Val- (m_rssi_cal)) / (10.0 * 2.35)));

        printf("Calculating Distance: %f", pow(10.0, Power));

        return pow(10.0, Power);
    }catch(std::exception X){
        printf("Error:: No Calibration Data Available");
        return 0;
    }
}

double Target_Node::getXCoord(){
    return m_XCoord;
}

double Target_Node::getYCoord(){
    return m_YCoord;
}

double Target_Node::getZCoord(){
    return m_ZCoord;
}

std::string Target_Node::getMAC(){
    return m_Node->getMAC();
}

bool Target_Node::getRecentlyUpdated(){
    m_Node->getRecentlyUpdated();
}

void Target_Node::setRecentlyUpdated(bool status){
    m_Node->setRecentlyUpdated(status);
}