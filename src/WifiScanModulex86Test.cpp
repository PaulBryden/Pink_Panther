//
// Created by green on 04/05/18.
//

#include "inc/Modules/WifiScanModulex86Test.h"

#ifndef FOO_WIFISCANMODULEX86TEST_CPP
#define FOO_WIFISCANMODULEX86TEST_CPP


WifiScanModulex86Test::WifiScanModulex86Test() : m_ScannedNodes(std::make_shared<node::NodeContainer>()),
                                                 m_ScanTime(0)
{

}

WifiScanModulex86Test::~WifiScanModulex86Test()
{

}

void WifiScanModulex86Test::initialize()
{
    if (!m_isRunning)
    {
        m_ScannerPtr = std::make_shared<boost::thread>(boost::bind(&WifiScanModulex86Test::Scan, this));
        m_isRunning = true;
    }
}

bool WifiScanModulex86Test::isRunning()
{
    return m_isRunning;
}

void WifiScanModulex86Test::deInitialize()
{
    if (m_isRunning)
    {
        m_isRunning = false;
        boost::mutex::scoped_lock lock(g_i_mutex);
        m_ScannerPtr->interrupt();
    }
}

std::shared_ptr<node::NodeContainer> WifiScanModulex86Test::getScannedNodes()
{

    boost::mutex::scoped_lock lock(g_i_mutex);
    return m_ScannedNodes;

}

double WifiScanModulex86Test::getScanTime()
{
    return m_ScanTime;
}

void WifiScanModulex86Test::Scan()
{
    while (m_isRunning)
    {

        boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
        boost::mutex::scoped_lock lock(g_i_mutex);
        boost::timer::auto_cpu_timer t;
        node::NodeContainer tempContainer;
        std::string Mac = "FF:FF:FF:FF:FF:FF";
        std::string ID = "eduroam";
        std::shared_ptr<Node> newNode = std::make_shared<Node>(ID, -61, "24:F2:7F:AA:2A:21");
        tempContainer.AddNode(newNode);
        ID = "TRIG2";
        newNode = std::make_shared<Node>(ID, -65, "24:F2:7F:AA:29:E1");
        tempContainer.AddNode(newNode);
        ID = "TRIG3";
        newNode = std::make_shared<Node>(ID, -78, "24:F2:2F:AA:26:E3");
        tempContainer.AddNode(newNode);
        ID = "TRIG4";
        newNode = std::make_shared<Node>(ID, -59, "24:F2:7F:AA:2C:01");
        tempContainer.AddNode(newNode);
        m_ScannedNodes->ClearNodes();
        for (auto &i : tempContainer.GetNodes())
        {
            m_ScannedNodes->AddNode(i);
        }

        m_ScanTime = t.elapsed().wall;

    }
}

#endif