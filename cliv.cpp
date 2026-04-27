//moduł klienta
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

//moduł komunikacji sieciowej
//moduł transferu plików
#include        "config_module.h" //moduł konfiguracji
#include        "command_module.h" //moduł protokołu aplikacyjnego

int main()
{
    //pobieranie konfiguracji od użytkownika
    config cfg; 
    if (config_get(cfg) < 0) {
        fprintf(stderr, "Błąd konfiguracji\n");
        return 1;
    }
    
    //TODO POŁĄCZENIE Z SERWEREM NA PODSTAWIE KONFIGURACJI (TCP LUB SCTP, ADRES IP)

    //odczytywanie komend od użytkownika
    char buf[100];
    while(scanf("%99s", buf) == 1)
    {
        command_handle(buf); 
    };
    return 0;
}