//
// Created by green on 25/10/17.
//

#include "inc/Modules/WifiScanModule.h"
#include "inc/iwlib.h"
#include <boost/timer/timer.hpp>
#pragma GCC diagnostic ignored "-Wwrite-strings"
#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <memory>
#include <list>
#include <cstdio>
#include <cstring>
#include "inc/Data/Node.h"
#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp>
WifiScanModule::~WifiScanModule()
{

}

WifiScanModule::WifiScanModule() : m_ScanTime(0.0), m_isRunning(false){


}


void WifiScanModule::initialize(){
    if(!m_isRunning) {
        m_ScannerPtr = std::make_shared<boost::thread>(boost::bind(&WifiScanModule::Scan, this));
        m_isRunning = true;
    }
}


void WifiScanModule::deInitialize(){
    if(m_isRunning) {
        m_isRunning = false;
        boost::mutex::scoped_lock lock(g_i_mutex);
        m_ScannerPtr->interrupt();
    }
}



std::shared_ptr<node::NodeContainer> WifiScanModule::getScannedNodes(){
    boost::mutex::scoped_lock lock(g_i_mutex);
    return m_ScannedNodes;
}


double WifiScanModule::getScanTime(){
    boost::mutex::scoped_lock lock(g_i_mutex);
    return m_ScanTime;
}


void WifiScanModule::Scan()
{
#ifdef 	__arm__

        wireless_scan_head head;
        wireless_scan *result;
        iwrange range;
        int sock;

        /* Open socket to kernel */
        sock = iw_sockets_open();
        while(m_isRunning){
            char		temp[128];
        boost::timer::auto_cpu_timer t;
        node::Node_Container tempContainer;
                /* Get some metadata to use for scanning */
                if (iw_get_range_info(sock, "wlan0", &range) < 0) {
                    printf("Error during iw_get_range_info. Aborting.\n");

                }else if (iw_scan(sock, "wlan0", range.we_version_compiled, &head) < 0) {
                    printf("Error during iw_scan.\n");

                }else {
                    /* Traverse the results */
                    result = head.result;
                    while (NULL != result) {

                        //printf("%s\n", result->b.essid);
                        // printf("%d\n",result->stats.qual.level);
                        int dbLevel = result->stats.qual.level;
                        if (dbLevel >= 64) {
                            dbLevel -= 0x100;
                        }
                        //printf("%d", dbLevel);
                        //result->b.freq,

                        boost::mutex::scoped_lock lock(g_i_mutex);
                        std::shared_ptr<Node> newNode = std::make_shared<Node>(result->b.essid, dbLevel, iw_saether_ntop(&result->ap_addr, temp));

                        tempContainer.AddNode(newNode);

                        result = result->next;

                    }

                    m_Nodes->ClearNodes();
                    for (auto &i : tempContainer.GetNodes()) {
                        m_Nodes->AddNode(i);
                    }
                    std::cout << "About to update Target Nodes.";
                    m_TargetNodes->UpdateNodes(m_Nodes);
                }


            m_scanTime=t.elapsed().wall;
            m_locMod->CalculateLocations(m_TargetNodes);
    }
#else

        while(m_isRunning) {
            boost::this_thread::sleep_for(boost::chrono::milliseconds(2000));
            boost::mutex::scoped_lock lock(g_i_mutex);
            boost::timer::auto_cpu_timer t;
            node::NodeContainer tempContainer;
            std::string Mac="FF:FF:FF:FF:FF:FF";
            std::string ID="TRIG1";
            std::shared_ptr<Node> newNode = std::make_shared<Node>(ID, -61,"24:F2:7F:AA:23:E1");
            tempContainer.AddNode(newNode);
            ID="TRIG2";
            newNode = std::make_shared<Node>(ID, -65, "18:64:72:34:06:00");
            tempContainer.AddNode(newNode);
            ID="TRIG3";
            newNode = std::make_shared<Node>(ID, -78, "24:F2:2F:AA:26:E3");
            tempContainer.AddNode(newNode);
            ID="TRIG4";
            newNode = std::make_shared<Node>(ID, -59, "24:F2:7F:AA:2C:01");
            tempContainer.AddNode(newNode);
            m_Nodes->ClearNodes();
            for (auto &i : tempContainer.GetNodes()) {
                m_Nodes->AddNode(i);
            }
            m_TargetNodes->UpdateNodes(m_Nodes);
            m_scanTime=t.elapsed().wall;
            m_locMod->CalculateLocations(m_TargetNodes);



        }
#endif

}






