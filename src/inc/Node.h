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
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
class Node: public INode {
private:
    std::string m_name;
    int m_rssi;
    std::string m_mac;
    bool m_Recently_Updated;
public:
    Node(std::string Name, int Rssi, std::string Mac);
    Node(web::json::value node);
    web::json::value ToJson();
    void Update(std::shared_ptr<INode> Node);
    int getRSSI() const;
    std::string getSSID();
    std::string getMAC();
    void setRecentlyUpdated(bool status);
    bool getRecentlyUpdated();

    inline bool operator< (const Node & c2)
    {
        return (this->getRSSI() < c2.getRSSI());
    }
    inline bool operator> (const Node & c2)
    {
        return (this->getRSSI() > c2.getRSSI());
    }
    inline bool operator<= (const Node & c2)
    {
        return (this->getRSSI() <= c2.getRSSI());
    }
    inline bool operator>= (const Node & c2)
    {
        return (this->getRSSI() >= c2.getRSSI());
    }
};


#endif //FOO_NODE_H
