#include        <sys/types.h>   /* basic system data types */
#include        <sys/socket.h>  /* basic socket definitions */
#include        <sys/time.h>    /* timeval{} for select() */
#include        <time.h>                /* timespec{} for pselect() */
#include        <netinet/in.h>  /* sockaddr_in{} and other Internet defns */
#include        <arpa/inet.h>   /* inet(3) functions */
#include        <errno.h>
#include        <fcntl.h>               /* for nonblocking */
#include        <netdb.h>
#include        <signal.h>
#include        <stdio.h>
#include        <stdlib.h>
#include        <string.h>
#include 	<unistd.h>

#include    "network_module.h" //moduł komunikacji sieciowej

#define TCP_PORT 2345
#define SCTP_PORT 6789
#define LISTENQ 2

socket_fds network_s_init()
{
    socket_fds fds;
    //tcp socket init
    fds.sctp_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fds.sctp_fd < 0) {
        perror("TCP socket");
        goto error;
    }
    //sctp socket init
    fds.tcp_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if (fds.tcp_fd < 0) {
        perror("SCTP socket");
        goto error;
    }

    //TCP socket bind and listen
    struct sockaddr_in tcp_addr;
    memset(&tcp_addr, 0, sizeof(tcp_addr));
    tcp_addr.sin_family = AF_INET;
    tcp_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    tcp_addr.sin_port = htons(TCP_PORT);

    if (bind(fds.tcp_fd, (struct sockaddr *)&tcp_addr, sizeof(tcp_addr)) < 0) {
        perror("TCP bind");
        goto error;
    }

    if (listen(fds.tcp_fd, LISTENQ) < 0) {
        perror("TCP listen");
        goto error;
    }

    //SCTP socket bind and listen
    struct sockaddr_in sctp_addr;
    memset(&sctp_addr, 0, sizeof(sctp_addr));
    sctp_addr.sin_family = AF_INET;
    sctp_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sctp_addr.sin_port = htons(SCTP_PORT);

    if (bind(fds.sctp_fd, (struct sockaddr *)&sctp_addr, sizeof(sctp_addr)) < 0) {
        perror("SCTP bind");
        goto error;
    }

    if (listen(fds.sctp_fd, LISTENQ) < 0) {
        perror("SCTP listen");
        goto error;
    }

    printf("Server ready: TCP:%d SCTP:%d\n", TCP_PORT, SCTP_PORT);
    return fds;

    error:
        if (fds.tcp_fd >= 0) close(fds.tcp_fd);
        if (fds.sctp_fd >= 0) close(fds.sctp_fd);
        return (socket_fds){-1, -1};
}

