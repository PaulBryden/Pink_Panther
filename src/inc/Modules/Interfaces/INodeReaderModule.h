//
// Created by green on 04/05/18.
//

#ifndef FOO_INODEREADER_H
#define FOO_INODEREADER_H
#include "../../Data/NodeContainer.h"
#include "IModule.h"
class INodeReaderModule: public IModule
{
public:
    virtual std::shared_ptr<node::NodeContainer> readNodes() = 0;
    virtual void initialize() = 0;
    virtual bool isRunning() = 0;
    virtual void deInitialize() = 0;
};
#endif //FOO_INODEREADER_H
