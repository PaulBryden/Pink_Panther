//
// Created by green on 29/10/17.
//

#ifndef FOO_TEST_SCANNER_H
#define FOO_TEST_SCANNER_H


#include <stdio.h>
#include <time.h>
#include <iwlib.h>

int main(void) {
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
        printf("%s\n", result->b.essid);
        printf("%s\n",result->stats.qual.level);
        result = result->next;
    }

    exit(0);
}