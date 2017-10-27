//
// Created by green on 25/10/17.
//

#ifndef FOO_NODE_H
#define FOO_NODE_H
#include <string>

class Node {

public:
    Node(std::string Name, std::string Mac, int SigStrength, int Rssi);
    Node();
        float approxDistance();
        std::string m_name;
        std::string m_Mac;
        int m_SigStrength;
        int m_Rssi;
};


#endif //FOO_NODE_H
