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

#include        "config_module.h" //moduł konfiguracji

int config_get(config &cfg)
{
    printf("Podaj protokół komunikacyjny (TCP lub SCTP): ");
    char protocol_str[10];
    scanf("%9s", protocol_str);
    if (strcmp(protocol_str, "TCP") == 0) 
    {
        cfg.protocol = 1;
    } 
    else if (strcmp(protocol_str, "SCTP") == 0) 
    {
        cfg.protocol = 2;
    } 
    else 
    {
        fprintf(stderr, "Nieznany protokół: %s\n", protocol_str);
        return -1;
    }

    printf("Podaj adres serwera (IPv4): ");
    char addr_str[INET_ADDRSTRLEN];
    scanf("%15s", addr_str);
    if (inet_pton(AF_INET, addr_str, &cfg.addr) <= 0) {
        fprintf(stderr, "Nieprawidłowy adres: %s\n", addr_str);
        return -1;
    }
    printf("Konfiguracja: protokół = %s, adres = %s\n", protocol_str, addr_str);
    return 0;
}
