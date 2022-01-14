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
    printf("[%s]\t", asctime(localtime()));
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void flog(FILE* f, const char *format, ...)
{
    va_list(args);
    fprintf(f, "[%s]\t", asctime(localtime()));
    va_start(args, format);
    vfprintf(f, format, args);
    va_end(args);
}