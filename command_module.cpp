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
#include        <cstdlib>
#include        <cstring>
#include "command_module.h"

void command_c_handle(const char *buf, int sockfd)
{
    char cmd[256];
    strncpy(cmd, buf, sizeof(cmd) - 1);
    cmd[sizeof(cmd) - 1] = '\0'; // Ensure null-termination
    //printf("%s\n", cmd);

    char *token = strtok(cmd, " ");

    if (strcmp(token, "HELP") == 0) {
        printf("Available commands:\n");
        printf("HELP - List available commands\n");
        printf("PUT - Put a file on the server\n");
        printf("EXIT - Exit the program\n");
    }
    else if (strcmp(token, "EXIT") == 0) {
        printf("Exiting the program.\n");
        exit(0);
    }
    else if (strcmp(token, "PUT") == 0 ){
        char *filename = strtok(NULL, " "); // Pobierz nazwę pliku po komendzie PUT
        if (filename == NULL) {
            printf("Użycie: PUT <nazwa_pliku>\n");
            return;
        }

        printf("Przygotowuję się do wysłania pliku: %s\n", filename);

        FILE *fp = fopen(filename, "rb");
        if (fp == NULL) {
            perror("Błąd otwarcia pliku lokalnego");
            return;
        }

        char buffer[1024];
        size_t n;
        printf("Wysyłanie pliku %s...\n", filename);

        // Prosty protokół: najpierw wysyłamy komendę, żeby serwer wiedział co robić
        send(sockfd, "PUT", 3, 0); 
        
        // Pętla czytająca plik i wysyłająca go w paczkach
        while ((n = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
            if (send(sockfd, buffer, n, 0) < 0) {
                perror("Błąd wysyłania");
                break;
            }
        }

        printf("Wysłano.\n");
        fclose(fp);

    } else {

        printf("No command found. HELP to list commands.\n");
        return;
    }
}