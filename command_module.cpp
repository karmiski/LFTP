#include <stdio.h>
#include "command_module.h"

void command_handle(const char *buf)
{
    printf("%s\n", buf);
}