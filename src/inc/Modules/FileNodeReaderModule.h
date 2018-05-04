//
// Created by green on 24/11/17.
//

#ifndef FOO_FILEIO_H
#define FOO_FILEIO_H

#include "../Data/NodeContainer.h"
#include "../Data/TargetNode.h"

#include <memory>
#include "Interfaces/INodeReaderModule.h"

class FileNodeReaderModule : public INodeReaderModule {
private:
    bool m_isRunning;


public:
    FileNodeReaderModule(std::string filepath);

    std::shared_ptr<node::NodeContainer> readNodes();
    void initialize();
    bool isRunning();
    std::string m_filepath;

};


#endif //FOO_FILEIO_H
