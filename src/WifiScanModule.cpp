//
// Created by green on 25/10/17.
//

#include "inc/WifiScanModule.h"
#include "inc/iwlib.h"
#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <memory>
#include <list>
#include <cstdio>
#include <cstring>


WifiScanModule::WifiScanModule(std::list<std::shared_ptr<Node>>& Nodes){
    iw_range range;
    const char* interfaceName = "wlan0";

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    //---------------------------------------------------------------------

    if ((iw_get_range_info(sockfd, interfaceName, &range) < 0) ||
        (range.we_version_compiled < 14))
    {
        printf("%s - interface doesn't support scanning\n", interfaceName);

        exit(EXIT_FAILURE);
    }

    __u8 wev = range.we_version_compiled;

    //---------------------------------------------------------------------

    struct iwreq request;

    request.u.param.flags = IW_SCAN_DEFAULT;
    request.u.param.value = 0;

    if (iw_set_ext(sockfd, interfaceName, SIOCSIWSCAN, &request) == -1)
    {
        perror("iw_set_ext(SIOCSIWSCAN)");
        exit(EXIT_FAILURE);
    }

    //---------------------------------------------------------------------

    struct timeval startTime;
    struct timeval endTime;
    struct timeval diffTime = { 0, 0 };

    bool replyFound = false;
    bool timeout = false;

    char scanBuffer[0xFFFF];

    gettimeofday(&startTime, NULL);

    while ((replyFound == false) && (timeout == false))
    {
        memset(scanBuffer, 0, sizeof(scanBuffer));
        request.u.data.pointer = scanBuffer;
        request.u.data.length = sizeof(scanBuffer);
        request.u.data.flags = 0;

        int result = iw_get_ext(sockfd,
                                interfaceName,
                                SIOCGIWSCAN,
                                &request);

        if (result == -1)
        {
            if (errno != EAGAIN)
            {
                perror("iw_get_ext(SICGIWSCAN)");
                exit(EXIT_FAILURE);
            }
        }

        if (result == 0)
        {
            replyFound = true;
        }

        gettimeofday(&endTime, NULL);
        timersub(&endTime, &startTime, &diffTime);

        if (diffTime.tv_sec > 10)
        {
            timeout = true;
        }
        else if (replyFound == false)
        {
            usleep(100000);
        }
    }

    close(sockfd);

    //---------------------------------------------------------------------

    bool firstAP = true;

    if (replyFound)
    {
        struct iw_event iwe;
        struct stream_descr stream;

        iw_init_event_stream(&stream,
                             scanBuffer,
                             request.u.data.length);

        char eventBuffer[512];
        std::shared_ptr<Node> newNode(std::make_shared<Node>());
        Nodes.push_back(newNode);

        while (iw_extract_event_stream(&stream, &iwe, wev) > 0)
        {

            switch(iwe.cmd)
            {
                case SIOCGIWAP:
                {
                    const struct ether_addr *eap =
                            (const struct ether_addr *)iwe.u.ap_addr.sa_data;

                    iw_ether_ntop(eap, eventBuffer);

                    if (firstAP)
                    {
                        firstAP = false;
                    }
                    else
                    {
                        printf("\n");
                    }

                    printf("%s ", eventBuffer);

                    break;
                }
                case SIOCGIWFREQ:
                {
                    double frequency = iw_freq2float(&(iwe.u.freq));
                    int channel = iw_freq_to_channel(frequency, &range);

                    if (frequency > 1.0)
                    {
                        frequency /= 1000000.0;
                    }

                    if (channel != -1)
                    {
                        printf("ch%02d ", channel);
                    }

                    break;
                }
                case SIOCGIWESSID:
                {
                    char essid[IW_ESSID_MAX_SIZE+10];
                    memset(essid, '\0', sizeof(essid));

                    if((iwe.u.essid.pointer) && (iwe.u.essid.length))
                    {
                        memcpy(essid,
                               iwe.u.essid.pointer,
                               iwe.u.essid.length);
                    }

                    if(iwe.u.essid.flags)
                    {
                        if((iwe.u.essid.flags & IW_ENCODE_INDEX) > 1)
                        {
                            char encodeIndex[10];
                            sprintf(encodeIndex,
                                    " [%d]",
                                    iwe.u.essid.flags & IW_ENCODE_INDEX);

                            strcat(essid, encodeIndex);
                        }
                    }
                    else
                    {
                        strcpy(essid, "hidden");
                    }

                    printf("%s ", essid);

                    newNode->m_name=essid;
                    /*Last SSID*/
                    newNode = std::make_shared<Node>();
                    Nodes.push_back(newNode);

                    break;
                }
                case IWEVQUAL:
                {
                    double signalQuality = (iwe.u.qual.qual * 100.0)
                                           / range.max_qual.qual;

                    printf("%.0f%% ", signalQuality);

                    if (iwe.u.qual.updated & IW_QUAL_RCPI)
                    {
                        double signalLevel = (iwe.u.qual.level / 2.0) - 110.0;

                        printf("%g dBm ", signalLevel);
                    }
                    else if (iwe.u.qual.updated & IW_QUAL_DBM)
                    {
                        int dbLevel = iwe.u.qual.level;

                        if (dbLevel >= 64)
                        {
                            dbLevel -= 0x100;
                        }

                        printf("%d dBm ", dbLevel);

                        newNode->m_Rssi=dbLevel;
                    }
                    else if ((iwe.u.qual.updated & IW_QUAL_LEVEL_INVALID) == 0)
                    {
                        printf("%d/%d ",
                               iwe.u.qual.level,
                               range.max_qual.level);
                    }

                    break;
                }
                default:

                    break;
            }
        }
    }

    if (firstAP == false)
    {
        printf("\n");
    }

    //---------------------------------------------------------------------
}