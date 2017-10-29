//
// Created by green on 25/10/17.
//

#ifndef FOO_WIFISCANMODULE_H
#define FOO_WIFISCANMODULE_H

#include "Node.h"
#include <memory>
#include <vector>
#include "iwlib.h"
#include <stdio.h>
#include <time.h>
class WifiScanModule {



public:
    WifiScanModule(std::vector<std::shared_ptr<Node>>& NodesList);
    ~WifiScanModule();
    void Scan();


private:
    std::vector<std::shared_ptr<Node>>& Nodes;
};

#endif //FOO_WIFISCANMODULE_H
