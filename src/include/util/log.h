#ifndef UTIL_LOG_H
#define UTIL_LOG_H

#include <stdio.h>

#define LOG_D 0
#define LOG_I 1
#define LOG_W 2
#define LOG_E 3


extern unsigned int min_log_level;

FILE* lopen(const char* path);
int lclose();
int printl(unsigned int log_level, const char* format, ...);


#endif // UTIL_LOG_H
