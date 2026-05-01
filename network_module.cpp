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
#include 	    <unistd.h>

#include    "network_module.h" //moduł komunikacji sieciowej
#include    "config_module.h" //moduł konfiguracji

#define MAXLINE 1024
#define TCP_PORT 2345
#define SCTP_PORT 6789
#define LISTENQ 2

socket_fds network_s_init()
{
    struct sockaddr_in tcp_addr, sctp_addr;
    int optval = 1;
    socket_fds fds;

    //tcp socket init
    fds.tcp_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fds.tcp_fd < 0) {
        perror("TCP socket");
        goto error;
    }
    //sctp socket init
    fds.sctp_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if (fds.sctp_fd < 0) {
        perror("SCTP socket");
        goto error;
    }

    // Opcja SO_REUSEADDR 
    setsockopt(fds.tcp_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    setsockopt(fds.sctp_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    //TCP socket bind and listen
    memset(&tcp_addr, 0, sizeof(tcp_addr)); //zero out structure
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
    memset(&sctp_addr, 0, sizeof(sctp_addr)); //zero out structure  
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

socket_fds network_s_accept(socket_fds fds)
{
    fd_set rset; //przechowuje deskryptory do monitorowania przez select()
    socket_fds client_fds = {-1, -1};
    int maxfdp1;

    FD_ZERO(&rset);
    FD_SET(fds.tcp_fd, &rset);
    FD_SET(fds.sctp_fd, &rset);

    // wyznaczamy najwyższy numer deskryptora dla select()
    maxfdp1 = (fds.tcp_fd > fds.sctp_fd ? fds.tcp_fd : fds.sctp_fd) + 1;

    printf("Oczekiwanie na połączenie (TCP/SCTP)...\n");

    // select() blokuje program do momentu aktywności na którymś z gniazd
    if (select(maxfdp1, &rset, NULL, NULL, NULL) < 0) {
        if (errno == EINTR) return client_fds; // Przerwanie przez sygnał
        perror("select error");
        return client_fds;
    }

    // sprawdzamy, które gniazdo zgłosiło gotowość
    if (FD_ISSET(fds.tcp_fd, &rset)) {
        struct sockaddr_in cliaddr;
        socklen_t len = sizeof(cliaddr);
        client_fds.tcp_fd = accept(fds.tcp_fd, (struct sockaddr *)&cliaddr, &len);
        printf("Nowe połączenie TCP z %s:%d\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
    } 
    else if (FD_ISSET(fds.sctp_fd, &rset)) {
        struct sockaddr_in cliaddr;
        socklen_t len = sizeof(cliaddr);
        client_fds.sctp_fd = accept(fds.sctp_fd, (struct sockaddr *)&cliaddr, &len);
        printf("Nowe połączenie SCTP z %s:%d\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
    }

    return client_fds;
    return (socket_fds){-1, -1};
}

int network_c_init(config &cfg)
{
    int sockfd;
    struct sockaddr_in servaddr;
   
    // Tworzenie gniazda
    sockfd = socket(AF_INET, SOCK_STREAM, cfg.protocol);
    if (sockfd < 0) {
        perror("Błąd tworzenia socketu");
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(cfg.port);
    servaddr.sin_addr = cfg.addr;

    const char* proto_name = (cfg.protocol == IPPROTO_SCTP) ? "SCTP" : "TCP";

    printf("Próba połączenia z %s:%d przez %s...\n", inet_ntoa(cfg.addr), cfg.port, proto_name);

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Połączenie nieudane");
        close(sockfd);
        return -1;
    }

    printf("Połączono pomyślnie (%s)!\n", proto_name);
    return sockfd;
}