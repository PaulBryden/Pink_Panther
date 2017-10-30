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
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
class WifiScanModule {



public:
    WifiScanModule(std::vector<std::shared_ptr<Node>>& NodesList);
    ~WifiScanModule();
    void Scan();


private:
    std::vector<std::shared_ptr<Node>>& Nodes;
    boost::mutex g_i_mutex;
};

#endif //FOO_WIFISCANMODULE_H
