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

        uint32_t cmd_len = strlen("PUT"); //stały rozmiar 4 bajty
        send(sockfd, &cmd_len, sizeof(cmd_len), 0); // Wysyłamy długość komendy
        send(sockfd, "PUT", 3, 0); //wysyłamy komendę PUT do serwera
        
        uint32_t name_len = strlen(filename); //długość nazwy pliku, 4 bajty
        send(sockfd, &name_len, sizeof(name_len), 0); // Wysyłamy długość nazwy pliku
        send(sockfd, filename, strlen(filename), 0); // Wysyłamy nazwę pliku do serwera

        fseek(fp, 0, SEEK_END); 
        uint32_t file_size = ftell(fp);
        rewind(fp);
        send(sockfd, &file_size, sizeof(file_size), 0);

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

void command_s_handle(int sockfd)
{
    char buffer[1024];
            ssize_t n;

            printf("--- Początek transmisji ---\n");
            
            // Pętla czytająca wszystko, co przysyła klient
            while (1) {

                uint32_t cmd_len;
                // odbierz rozmiar komendy (4 bajty)
                if (read(sockfd, &cmd_len, sizeof(cmd_len)) != sizeof(cmd_len)) {
                    perror("Błąd odczytu rozmiaru komendy");
                    return;
                }

                // odbierz samą komendę
                char cmd[cmd_len + 1];
                read(sockfd, cmd, cmd_len);
                cmd[cmd_len] = '\0';

                if (strcmp(cmd, "PUT") == 0) {
                    // odbierz rozmiar nazwy pliku (4 bajty)
                    uint32_t name_len;
                    read(sockfd, &name_len, sizeof(name_len));

                    // dbierz nazwę pliku
                    char filename[name_len + 1];
                    read(sockfd, filename, name_len);
                    filename[name_len] = '\0';

                    printf("Otrzymano zlecenie PUT dla pliku: %s\n", filename);

                    //stworz nowy plik do zapisu
                    FILE *fp = fopen(filename, "wb");
                    if (fp == NULL) {
                        perror("Błąd otwarcia pliku do zapisu");
                        return;
                    }
                    //zapisz do pliku wszystko, co klient wysyła
                    uint32_t file_size;
                    read(sockfd, &file_size, sizeof(file_size));
                    uint32_t bytes_received = 0;
                    while (bytes_received < file_size) {
                        n = read(sockfd, buffer, sizeof(buffer));
                        if (n <= 0) {
                            perror("Błąd odczytu danych");
                            break;
                        }
                        fwrite(buffer, 1, n, fp);
                        bytes_received += n;
                    }
                    fclose(fp);
                    printf("Otrzymano i zapisano plik %s.\n", filename);
                }
            }

            if (n < 0) perror("Błąd read()");
            
            printf("\n--- Koniec transmisji (Klient rozłączony) ---\n");
            close(sockfd);
}