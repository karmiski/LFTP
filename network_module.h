#ifndef NETWORK_MODULE_H
#define NETWORK_MODULE_H

struct socket_fds
{
        int tcp_fd = -1;
        int sctp_fd = -1;
};

socket_fds network_s_init();

#endif