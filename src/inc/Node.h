//
// Created by green on 25/10/17.
//

#ifndef FOO_NODE_H
#define FOO_NODE_H
#include <string>
#include <cpprest/http_client.h>
#include <iostream>
#include <cpprest/json.h>
class Node {

public:
    Node(std::string Name, std::string Mac, int SigStrength, int Rssi);
    Node(web::json::value node);
    Node();
    web::json::value ToJson();
        float approxDistance();
        std::string m_name;
        std::string m_Mac;
        int m_SigStrength;
        double x_coord;
        double y_coord;
        int m_Rssi;
        double m_channel;
        double m_RssiCalib;
    double CalculateDistance();
};


#endif //FOO_NODE_H
