//
// Created by green on 25/10/17.
//

#include "inc/Modules/WifiScanModule.h"

#pragma GCC diagnostic ignored "-Wwrite-strings"

#include <boost/chrono.hpp>

WifiScanModule::~WifiScanModule()
{
    deInitialize();
}

WifiScanModule::WifiScanModule(std::shared_ptr<node::NodeContainer>& nodes) : targetNodes(nodes),
									      m_ScannedNodes(std::make_shared<node::NodeContainer>()), m_ScanTime(0.0), m_isRunning(false)
{

}

void WifiScanModule::initialize()
{
  if (!m_isRunning)
    {
        m_ScannerPtr = std::make_shared<boost::thread>(boost::bind(&WifiScanModule::Scan, this));
        m_isRunning = true;
    }
}

void WifiScanModule::deInitialize()
{
    if (m_isRunning)
    {
        m_isRunning = false;
        boost::mutex::scoped_lock lock(g_i_mutex);
        m_ScannerPtr->interrupt();
    }
}


std::shared_ptr<node::NodeContainer> WifiScanModule::getScannedNodes()
{
    boost::mutex::scoped_lock lock(g_i_mutex);
    return m_ScannedNodes;
}

double WifiScanModule::getScanTime()
{
    boost::mutex::scoped_lock lock(g_i_mutex);
    return m_ScanTime;
}

bool WifiScanModule::isRunning()
{
    return m_isRunning;
}

void WifiScanModule::Scan()
{
    wireless_scan_head head;
    wireless_scan *result;
    iwrange range;
    int sock;

    /* Open socket to kernel */
    sock = iw_sockets_open();
    while (m_isRunning)
    {
        boost::mutex::scoped_lock lock(g_i_mutex);
        char temp[128];
        boost::timer::auto_cpu_timer t;
        node::NodeContainer tempContainer;
        /* Get some metadata to use for scanning */
        if (iw_get_range_info(sock, "wlan0", &range) < 0)
        {
            printf("Error during iw_get_range_info. Aborting.\n");
        } else if (iw_scan(sock, "wlan0", range.we_version_compiled, &head) < 0)
        {
            printf("Error during iw_scan.\n");
        } else
        {
            /* Traverse the results */
            result = head.result;
            while (NULL != result)
            {
                int dbLevel = result->stats.qual.level;
                if (dbLevel >= 64)
                {
                    dbLevel -= 0x100;
                }
                std::shared_ptr<Node> newNode = std::make_shared<Node>(result->b.essid, dbLevel,
                                                                       iw_saether_ntop(&result->ap_addr, temp));
                tempContainer.AddNode(newNode);

		// std::cerr << "Found scanned node " << result->b.essid << ", " << dbLevel << " dBm" << std::endl;

		result = result->next;
            }
	    m_ScannedNodes->ClearNodes();
	    for (auto &i : tempContainer.GetNodes())
            {
	      if(!strncmp("TRIG",i->getSSID().c_str(),4))
		{
		  m_ScannedNodes->AddNode(i);
		  // std::cerr << "Added scanned node " << i->getSSID() << ", " << i->getRSSI() << " dBm, " << i->getMAC() << std::endl;
		}
            }
            // std::cout << "About to update Target Nodes.";
        }
        m_ScanTime = t.elapsed().wall;
    }
}
