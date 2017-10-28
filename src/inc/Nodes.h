//
// Created by green on 25/10/17.
//

#ifndef FOO_NODES_H
#define FOO_NODES_H
#include "Node.h"
#include <list>
#include <memory>
#include "WifiScanModule.h"

class Nodes {

public:
    Nodes();
    web::json::value ToJson();
        WifiScanModule m_Scanner;
        std::list<std::shared_ptr<Node>> m_Nodes;
        void RefreshNodes();
        void PrintNodes();



};


#endif //FOO_NODES_H
