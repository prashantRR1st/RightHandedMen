#ifndef RHM_PLATFORM_H
#define RHM_PLATFORM_H

typedef int Time;

/**
 * @brief OS/MCU system time call
 * 
 * @return int time
 */
Time timer()
{

    return 0;
}

/**
 * @brief OS/MCU system sleep call
 * 
 */
void sleep()
{
}



void log(const char *format, ...)
{
    va_list(args);
    printf("[%s]\t", now())
}

#endif