//
// Created by green on 24/11/17.
//

#ifndef FOO_FILEIO_H
#define FOO_FILEIO_H
#include "Nodes.h"
#include "Node.h"

#include <memory>
namespace FileIO {
    class FileIO{


    public:
        FileIO(node::Nodes* nodeList);
        void WriteOut();
        void ReadIn();
        std::shared_ptr<node::Nodes> m_NodeList;

    };
}


#endif //FOO_FILEIO_H
