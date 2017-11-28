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
double Target_Node::CalculateDistance() {
    printf("Calculating range base on RSSI of:%d",m_Node->getRSSI());
    try {
        double Power = (-(static_cast<double>(m_Node->getRSSI()) - (m_RssiCalib)) / (10.0 * 3.0));

        printf("Calculating Distance: %f", pow(10.0, Power));

        return pow(10.0, Power);
    }catch(std::exception X){
        printf("Error:: No Calibration Data Available");
        return 0;
    }
}

void Target_Node::Update(std::shared_ptr<INode> Node){
    m_Node=Node;
}

Target_Node::Target_Node(web::json::value node){


    m_Node=new std::shared_ptr(std::make_shared(new Node(node)));

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

int Target_Node::getRSSI(){
    return m_Node->getRSSI();
}

double Target_Node::getChannel(){
    return m_Node->getChannel();
}

std::string Target_Node::getSSID(){
    return m_Node->getSSID();
}