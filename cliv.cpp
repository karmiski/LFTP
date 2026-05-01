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
#include        "network_module.h" //moduł komunikacji sieciowej
int main()
{
    //pobieranie konfiguracji od użytkownika
    config cfg; 
    if (config_get(cfg) < 0) {
        fprintf(stderr, "Błąd konfiguracji\n");
        return 1;
    }
    
    int sockfd = network_c_init(cfg);
    if (sockfd < 0) {
        fprintf(stderr, "Błąd połączenia z serwerem\n");
        return 1;
    }
    else
    {
        char welcome_buf[1024];
        ssize_t n = read(sockfd, welcome_buf, sizeof(welcome_buf) - 1);
        if (n > 0) {
        welcome_buf[n] = '\0'; // Terminacja stringa
        printf("Serwer mówi: %s", welcome_buf);
        }
    }

    //odczytywanie komend od użytkownika
    char buf[100];

    while(fgets(buf, sizeof(buf), stdin) != NULL) {
        // fgets zostawia znak nowej linii na końcu
        buf[strcspn(buf, "\n")] = 0;

        if (strlen(buf) > 0) {
            command_c_handle(buf, sockfd);
        }
        printf("ftp> ");
    }
    return 0;
}