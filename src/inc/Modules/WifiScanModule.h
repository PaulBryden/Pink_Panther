//
// Created by green on 25/10/17.
//

#ifndef FOO_WIFISCANMODULE_H
#define FOO_WIFISCANMODULE_H

#include <memory>
#include <vector>
#include "../iwlib.h"
#include <stdio.h>
#include <time.h>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include "../Data/NodeContainer.h"
#include "LocationModule.h"
#include "Interfaces/IScanModule.h"
using namespace node;
class WifiScanModule : public IScanModule {

public:
    WifiScanModule();
    ~WifiScanModule();

    void initialize();
    bool isRunning();
    void deInitialize();
    std::shared_ptr<node::NodeContainer> WifiScanModule::getScannedNodes();
    double WifiScanModule::getScanTime();
private:
    bool m_isRunning;
    void Scan();
    std::shared_ptr<node::NodeContainer> m_ScannedNodes;
    double m_ScanTime;
    boost::mutex g_i_mutex;
    std::shared_ptr<boost::thread> m_ScannerPtr;
};

#endif //FOO_WIFISCANMODULE_H
