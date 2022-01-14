#ifndef RHM_PLATFORM_H
#define RHM_PLATFORM_H

#include <stdio.h>

typedef int Time;

/**
 * @brief OS/MCU system time call
 * 
 * @return int time
 */
Time timer();

/**
 * @brief OS/MCU system sleep call
 * 
 */
void sleep();



/**
 * @brief OS/MCU system print call
 * 
 * @param format 
 * @param ... 
 */
void log(const char *format, ...);

/**
 * @brief OS/MCU system file write
 * 
 * @param format 
 * @param ... 
 */
void flog(FILE* f, const char *format, ...);

#endif