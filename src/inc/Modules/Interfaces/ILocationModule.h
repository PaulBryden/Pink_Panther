//
// Created by green on 04/05/18.
//

#ifndef FOO_ILOCATIONMODULE_H
#define FOO_ILOCATIONMODULE_H

#include "../../Data/NodeContainer.h"
#include "../../Data/TargetNode.h"
#include "../../Data/Location.h"
#include "IModule.h"
class ILocationModule : public IModule
{
public:

    virtual web::json::value GetJson() = 0;
    virtual std::shared_ptr<Location> GetLocations() = 0;
    virtual void initialize() = 0;
    virtual bool isRunning() = 0;
    virtual void deInitialize() = 0;
};
#endif //FOO_ILOCATIONMODULE_H
