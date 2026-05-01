#ifndef CONFIG_MODULE_H
#define CONFIG_MODULE_H

#include <netinet/in.h>

struct config
{
    int port; //port serwera, TCP = 2345, SCTP = 6789
    struct in_addr addr; //adres serwera (IPv4)
};

int config_get(config &cfg);

#endif