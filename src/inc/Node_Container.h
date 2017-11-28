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
    class Node_Container {

    public:

        Node_Container();

        web::json::value ToJson();

        void PrintNodes();

        void ClearNodes();

        std::vector<std::shared_ptr<INode>>& GetNodes();

        void AddNode(std::shared_ptr<INode> node);

        void UpdateNodes(std::shared_ptr<Node_Container> nodes);

    private:
        boost::mutex g_i_mutex;
        std::vector<std::shared_ptr<INode>> m_Nodes;

    };
}

#endif //FOO_NODES_H
