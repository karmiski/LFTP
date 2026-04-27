#ifndef CONFIG_MODULE_H
#define CONFIG_MODULE_H

#include <netinet/in.h>

struct config
{
    int protocol = 0; //protokół komunikacyjny (TCP = 1, SCTP = 2)
    struct in_addr addr; //adres serwera (IPv4)
};

int config_get(config &cfg);

#endif