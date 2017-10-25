//
// Created by green on 25/10/17.
//

#ifndef FOO_NODES_H
#define FOO_NODES_H
#include "Node.h"
#include <List>

class Nodes {
    Nodes();

    public:
        List<std::shared_ptr<Node>> m_Nodes;
        void RefreshNodes();

};


#endif //FOO_NODES_H
