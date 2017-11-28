//
// Created by green on 25/10/17.
//

#ifndef FOO_NODE_H
#define FOO_NODE_H
#include <string>
#include <cpprest/http_client.h>
#include <iostream>
#include <cpprest/json.h>
#include "INode.h"
#include <atomic>
class Node: public INode {
private:
    std::string m_name;
    double m_channel;
    int m_Rssi;
public:
    Node(std::string Name, int Rssi, double Channel);
    Node(web::json::value node);
    web::json::value ToJson();
    void Update(std::shared_ptr<INode> Node);
    int getRSSI();
    double getChannel();
    std::string getSSID();

};


#endif //FOO_NODE_H
