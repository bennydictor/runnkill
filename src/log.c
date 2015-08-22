#include <log.h>
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>



#define LOG_MESGS_SIZE 4

char* mesgs[LOG_MESGS_SIZE] = {
    "(DD)",
    "(II)",
    "(WW)",
    "(EE)"   
};

unsigned int min_log_level = 0;
static FILE* log_file_fd;
static time_t begin_t;

FILE* lopen(const char* path) {
    time(&begin_t);
    return log_file_fd = fopen(path, "w");
}

int lclose() {
    return fclose(log_file_fd);
}

int printl(unsigned int log_level, const char* format, ...) {
    if (log_level < min_log_level) {
        return 0;
    }
    int ret;
    va_list args;
    time_t current_t;
    time(&current_t);
    fprintf(log_file_fd, "[%10.10f] ", difftime(current_t, begin_t));
    if (log_level < LOG_MESGS_SIZE) {
        fprintf(log_file_fd, "%s ", mesgs[log_level]);
    } else {
        fprintf(log_file_fd, "(%2d) ", log_level);
    }
    va_start(args, format);
    ret = vfprintf(log_file_fd, format, args);
    va_end(args);
    fflush(log_file_fd);
    return ret;
}

int printlm(unsigned int log_level, const char* format, ...) {
    if (log_level < min_log_level) {
        return 0;
    }
    int ret;
    va_list args;
    va_start(args, format);
    ret = vfprintf(log_file_fd, format, args);
    va_end(args);
    fflush(log_file_fd);
    return ret;
}
