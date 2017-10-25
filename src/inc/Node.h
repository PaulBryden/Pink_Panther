//
// Created by green on 25/10/17.
//

#ifndef FOO_NODE_H
#define FOO_NODE_H


class Node {

    Node(String Name, String Mac, int SigStrength, int Rssi);
    Node();

    public:
        float approxDistance();
        String m_name;
        String m_Mac;
        int m_SigStrength;
        int m_Rssi;
};


#endif //FOO_NODE_H
