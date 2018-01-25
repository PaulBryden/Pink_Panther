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
    m_RssiVec;
}

Target_Node::Target_Node(web::json::value node){
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

double Target_Node::CalculateDistance() {
    if(m_RssiVec.size()==12) {
        m_RssiVec.pop_front();
    }
    m_RssiVec.push_back(m_Node->getRSSI());

    std::deque<int> y(m_RssiVec);
    std::sort(y.begin(),y.end());
    int RSSI=y[((m_RssiVec.size()/2)-1) +1];
    cout<< RSSI;

    printf("Calculating range base on RSSI of:%d",RSSI);
    try {
        double Power = (-(static_cast<double>(RSSI - (m_RssiCalib)) / (10.0 * 3.3)));

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