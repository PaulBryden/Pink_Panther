//
// Created by green on 25/10/17.
//

#ifndef FOO_NODES_H
#define FOO_NODES_H
#include "Node.h"
#include <vector>
#include <memory>
#include "WifiScanModule.h"
#include <string>

namespace node {
    class Nodes {

    public:
        std::vector<std::shared_ptr<Node>> m_Nodes;

        Nodes();

        web::json::value ToJson();

        WifiScanModule m_Scanner;

        void RefreshNodes();

        void PrintNodes();


    };
}

#endif //FOO_NODES_H
