//
// Created by green on 04/12/17.
//

#ifndef FOO_LOCATIONMODULE_H
#define FOO_LOCATIONMODULE_H
#include "Node_Container.h"
#include "Target_Node.h"
#include "Location.h"
#include <lapacke.h>
class locationModule {
public:
    std::shared_ptr<Location>  calculateDgels(std::shared_ptr<node::Node_Container> nodes);


};


#endif //FOO_LOCATIONMODULE_H
