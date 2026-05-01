#ifndef NETWORK_MODULE_H
#define NETWORK_MODULE_H

#include    "config_module.h" //moduł konfiguracji

struct socket_fds
{
        int tcp_fd = -1;
        int sctp_fd = -1;
};

//funkcje serwera
socket_fds network_s_init();
socket_fds network_s_accept(socket_fds fds);

//funkcje klienta
int network_c_init(config &cfg);
#endif