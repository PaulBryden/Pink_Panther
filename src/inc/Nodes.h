//
// Created by green on 25/10/17.
//

#ifndef FOO_NODES_H
#define FOO_NODES_H
#include "Node.h"
#include <list>
#include <memory>

class Nodes {
    Nodes();

    public:
        std::list<std::shared_ptr<Node>> m_Nodes;
        void RefreshNodes();

};


#endif //FOO_NODES_H
