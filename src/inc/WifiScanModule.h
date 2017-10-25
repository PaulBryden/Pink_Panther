//
// Created by green on 25/10/17.
//

#ifndef FOO_WIFISCANMODULE_H
#define FOO_WIFISCANMODULE_H

#include "inc/iwlib.h"
#include <unistd.h>

#include <sys/socket.h>
#include <sys/time.h>

#include <cstdio>
#include <cstring>
#include "inc/Node"
#include "inc/Nodes"

class WifiScanModule {


    WifiScanModule(std::list<std::shared_ptr<Node>>& Nodes);
};

#endif //FOO_WIFISCANMODULE_H
