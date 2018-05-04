//
// Created by green on 04/05/18.
//

#ifndef FOO_ISCANMODULE_H
#define FOO_ISCANMODULE_H
#include "../../Data/NodeContainer.h"
#include "IModule.h"
class IScanModule : public IModule
{
public:
    virtual std::shared_ptr<node::NodeContainer> getScannedNodes() = 0;
    virtual double getScanTime() = 0;
    virtual void initialize() = 0;
    virtual bool isRunning() = 0;
    virtual void deInitialize() = 0;

};
#endif //FOO_ISCANMODULE_H
