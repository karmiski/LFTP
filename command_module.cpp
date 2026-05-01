#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include "command_module.h"

void command_handle(const char *buf)
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
        //TODO
    } else {
        printf("No command found. HELP to list commands.\n");
        return;
    }
}