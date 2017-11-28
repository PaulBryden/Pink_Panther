//
// Created by green on 24/11/17.
//

#ifndef FOO_FILEIO_H
#define FOO_FILEIO_H
#include "Node_Container.h"
#include "Target_Node.h"

#include <memory>
namespace FileIO {
    class FileIO{


    public:
        FileIO(std::shared_ptr<node::Node_Container> nodeList);
        void WriteOut();
        void ReadIn();
        std::shared_ptr<node::Node_Container> m_NodeList;

    };
}


#endif //FOO_FILEIO_H
