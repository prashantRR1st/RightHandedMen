#include <stdio.h>
#include <time.h>
#include "platform.h"

Time timer()
{

    return 0;
}

void sleep()
{
}

void log(const char *format, ...)
{
    va_list(args);
    printf("[%s]\t", );
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}