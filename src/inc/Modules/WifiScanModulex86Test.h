//
// Created by green on 04/05/18.
//

#ifndef FOO_WIFISCANMODULEX86TEST_H
#define FOO_WIFISCANMODULEX86TEST_H

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

class WifiScanModulex86Test : public IScanModule
{

public:
    WifiScanModulex86Test();

    ~WifiScanModulex86Test();

    void initialize();

    bool isRunning();

    void deInitialize();

    std::shared_ptr<node::NodeContainer> getScannedNodes();

    double getScanTime();

private:
    bool m_isRunning;

    void Scan();

    std::shared_ptr<node::NodeContainer> m_ScannedNodes;
    double m_ScanTime;
    boost::mutex g_i_mutex;
    std::shared_ptr<boost::thread> m_ScannerPtr;
};

#endif //FOO_WIFISCANMODULEX86TEST_H
