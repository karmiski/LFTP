#ifndef CONFIG_MODULE_H
#define CONFIG_MODULE_H

#include <netinet/in.h>

struct config
{
    int port; //port serwera, TCP = 2345, SCTP = 6789
    int protocol; //0 dla TCP, IPPROTO_SCTP dla SCTP
    struct in_addr addr; //adres serwera (IPv4)
};

int config_get(config &cfg);

#endif