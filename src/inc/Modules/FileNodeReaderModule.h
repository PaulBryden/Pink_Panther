//
// Created by green on 24/11/17.
//

#ifndef FOO_FILEIO_H
#define FOO_FILEIO_H

#include "../Data/NodeContainer.h"
#include "../Data/TargetNode.h"

#include <memory>
#include "Interfaces/INodeReaderModule.h"

class FileNodeReaderModule : public INodeReaderModule
{
private:
    bool m_isRunning;
    std::string m_filepath;


public:
    FileNodeReaderModule(std::string filepath);

    ~FileNodeReaderModule();

    std::shared_ptr<node::NodeContainer> readNodes();

    boost::mutex g_i_mutex;

    void initialize();

    void deInitialize();

    bool isRunning();

};


#endif //FOO_FILEIO_H
