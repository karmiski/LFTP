//moduł serwera
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

#include        "network_module.h" //moduł komunikacji sieciowej
//TCP - port 2345
//SCTP - port 6789
int main()
{
    socket_fds fds = network_s_init();
    if(fds.tcp_fd < 0 || fds.sctp_fd < 0) {
        fprintf(stderr, "Błąd podczas inicjalizacji modułu komunikacji sieciowej\n");
        return 1;
    }

    return 0;
}