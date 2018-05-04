//
// Created by green on 25/10/17.
//

#ifndef FOO_NODES_H
#define FOO_NODES_H

#include "../Interfaces/INode.h"
#include <vector>
#include <memory>
#include <string>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>


namespace node
{
    class NodeContainer
    {

    public:

        NodeContainer();

        web::json::value ToJson();

        void PrintNodes();

        void ClearNodes();

        std::vector<std::shared_ptr<INode>> &GetNodes();

        void AddNode(std::shared_ptr<INode> node);

        void UpdateNodes(std::shared_ptr<NodeContainer> nodes);

    private:
        boost::mutex g_i_mutex;
        std::vector<std::shared_ptr<INode>> m_Nodes;

    };
}

#endif //FOO_NODES_H
