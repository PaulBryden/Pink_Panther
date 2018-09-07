//
// Created by green on 04/05/18.
//

#ifndef FOO_IMODULE_H
#define FOO_IMODULE_H

class IModule
{

public:
    virtual void initialize() = 0;

    virtual bool isRunning() = 0;

    virtual void deInitialize() = 0;
};

#endif //FOO_IMODULE_H
