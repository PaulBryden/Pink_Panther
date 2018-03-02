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
            m_XCoord = node["XCoord"].as_double();
            cout << node["XCoord"];
        }catch(std::exception e) {
            printf("Error:: Cannot parse XCoord from JSON. Please check Syntax");
            return;
        }
    try {
        m_YCoord = node["YCoord"].as_double();
        cout << node["YCoord"];
    }catch(std::exception e) {
        printf("Error:: Cannot parse YCoord from JSON. Please check Syntax");
        return;
    }
    try {
        m_ZCoord = node["ZCoord"].as_double();
        cout << node["ZCoord"];

    } catch(std::exception e) {
            printf("Error:: Cannot parse ZCoord from JSON. Please check Syntax");
            return;
        }
    try{
        m_RssiCalib = node["RSSICalib"].as_double();
        cout << node["RSSICalib"];
    }  catch(std::exception e) {
        printf("Error:: Cannot parse RSSICalib from JSON. Please check Syntax");
        return;
    }



}

web::json::value Target_Node::ToJson(){
    //TO BE POPULATED
    using namespace web;
    json::value response = m_Node->ToJson();
    //value::parse(U("{ \"ssid\" : \""+m_name+"\", \"m_rssi\" : "+std::to_string(m_Rssi)+" }"));

    response["XCoord"] = json::value::number(m_XCoord);
    response["YCoord"] = json::value::number(m_YCoord);
    response["ZCoord"] = json::value::number(m_ZCoord);
    response["RSSICalib"] = json::value::number(m_RssiCalib);
    response["Distance"] = json::value::number(CalculateDistance());
    response["RSSIFiltered"] = json::value::number(m_Kalman_RSSI_Val);
    //response["KalmanDistance"] = json::value::number(m_Kalman_Distance);
   // cout <<response.serialize();

    return response;


}

int Target_Node::getRSSI(){
    return m_Node->getRSSI();
}


double Target_Node::getChannel(){
    return m_Node->getChannel();
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
        double Power = (-(static_cast<double>(m_Kalman_RSSI_Val- (m_RssiCalib)) / (10.0 * 2.35)));

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