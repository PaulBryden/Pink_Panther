//
// Created by green on 25/10/17.
//

#include "inc/WifiScanModule.h"
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
#include "inc/Node.h"
#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp>
WifiScanModule::~WifiScanModule()
{

}

WifiScanModule::WifiScanModule(std::shared_ptr<node::Node_Container> ScannedNodesList, std::shared_ptr<node::Node_Container> Target_Nodes,std::shared_ptr<locationModule>& locMod,double& scanTime)
        : m_Nodes(ScannedNodesList),m_TargetNodes(Target_Nodes),m_locMod(locMod),m_scanTime(scanTime)
{



}

void WifiScanModule::Scan()
{
#ifdef 	__arm__
        boost::mutex::scoped_lock lock(g_i_mutex);

        wireless_scan_head head;
        wireless_scan *result;
        iwrange range;
        int sock;

        /* Open socket to kernel */
        sock = iw_sockets_open();
        while(true){
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

                        printf("%s\n", result->b.essid);
                        // printf("%d\n",result->stats.qual.level);
                        int dbLevel = result->stats.qual.level;
                        printf("AP ADDR::%s",iw_saether_ntop(&result->ap_addr, temp));
                        if (dbLevel >= 64) {
                            dbLevel -= 0x100;
                        }
                        printf("%d", dbLevel);

                        std::shared_ptr<Node> newNode = std::make_shared<Node>(result->b.essid, dbLevel, result->b.freq);

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

        boost::mutex::scoped_lock lock(g_i_mutex);
        while(true) {
            boost::timer::auto_cpu_timer t;
            node::Node_Container tempContainer;
            std::shared_ptr<Node> newNode = std::make_shared<Node>("TRIG1", -51, 1500);
            tempContainer.AddNode(newNode);
            newNode = std::make_shared<Node>("TRIG2", -55, 1501);
            tempContainer.AddNode(newNode);
            newNode = std::make_shared<Node>("TRIG3", -58, 1502);
            tempContainer.AddNode(newNode);
            newNode = std::make_shared<Node>("TRIG4", -59, 1503);
            tempContainer.AddNode(newNode);
            m_Nodes->ClearNodes();
            for (auto &i : tempContainer.GetNodes()) {
                m_Nodes->AddNode(i);
            }
            m_TargetNodes->UpdateNodes(m_Nodes);

            m_scanTime=t.elapsed().wall;
            m_locMod->CalculateLocations(m_TargetNodes);


            boost::this_thread::sleep_for(boost::chrono::milliseconds(2000));

        }
#endif

}






