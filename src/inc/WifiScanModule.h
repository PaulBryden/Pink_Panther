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
#include "Node_Container.h"
class WifiScanModule {



public:
    WifiScanModule(std::shared_ptr<node::Node_Container> ScannedNodesList, std::shared_ptr<node::Node_Container> Target_Nodes));
    ~WifiScanModule();
    void Scan();


private:
    std::shared_ptr<node::Node_Container> m_Nodes;
    std::shared_ptr<node::Node_Container> m_TargetNodes;
    boost::mutex g_i_mutex;
};

#endif //FOO_WIFISCANMODULE_H
