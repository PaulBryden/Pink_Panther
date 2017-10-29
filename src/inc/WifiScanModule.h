//
// Created by green on 25/10/17.
//

#ifndef FOO_WIFISCANMODULE_H
#define FOO_WIFISCANMODULE_H

#include "Node.h"
#include <memory>
#include <vector>
#include "iwlib.h"
class WifiScanModule {



public:
    WifiScanModule(std::vector<std::shared_ptr<Node>>& NodesList);
    ~WifiScanModule();
    void Scan();
    iw_range range;
    const char* interfaceName = "wlan0";

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    struct iwreq request;
    __u8 wev;

    //-----

private:
    std::vector<std::shared_ptr<Node>>& Nodes;
};

#endif //FOO_WIFISCANMODULE_H
