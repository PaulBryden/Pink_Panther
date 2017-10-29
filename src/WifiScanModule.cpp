//
// Created by green on 25/10/17.
//

#include "inc/WifiScanModule.h"
#pragma GCC diagnostic ignored "-Wwrite-strings"
#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <memory>
#include <list>
#include <cstdio>
#include <cstring>

WifiScanModule::~WifiScanModule()
{

}

WifiScanModule::WifiScanModule(std::vector<std::shared_ptr<Node>>& NodesList) : Nodes(NodesList)
{



}
void WifiScanModule::Scan()
{

    std::shared_ptr<Node> newNode(std::make_shared<Node>());

    wireless_scan_head head;
    wireless_scan *result;
    iwrange range;
    int sock;

    /* Open socket to kernel */
    sock = iw_sockets_open();

    /* Get some metadata to use for scanning */
    if (iw_get_range_info(sock, "wlan0", &range) < 0) {
        printf("Error during iw_get_range_info. Aborting.\n");
        exit(2);
    }

    /* Perform the scan */
    if (iw_scan(sock, "wlan0", range.we_version_compiled, &head) < 0) {
        printf("Error during iw_scan. Aborting.\n");
        exit(2);
    }

    /* Traverse the results */
    result = head.result;
    while (NULL != result) {

        newNode = std::make_shared<Node>();
        Nodes.push_back(newNode);
        printf("%s\n", result->b.essid);
        // printf("%d\n",result->stats.qual.level);
        int dbLevel = result->stats.qual.level;
        if (dbLevel >= 64)
        {
            dbLevel -= 0x100;
        }
        printf("%d",dbLevel);

        newNode->m_name=result->b.essid;

        newNode->m_Rssi=dbLevel;

        result = result->next;
    }
}

