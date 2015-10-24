#include <util/log.h>

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
time_t begin_t;

FILE* lopen(const char* path) {
    time(&begin_t);
    return log_file_fd = fopen(path, "w");
}

int lclose() {
    return fclose(log_file_fd);
}

void printl_header(char *buf, int time, int level) {
    sprintf(buf, "[%010d] ", time);
    buf += strlen(buf);
    if (level < LOG_MESGS_SIZE) {
        sprintf(buf, "%s ", mesgs[level]);
    } else {
        sprintf(buf, "(%2d) ", level);
    }
}

int printl(unsigned int log_level, const char* format, ...) {
    if (log_level < min_log_level) {
        return 0;
    }
    time_t current_t;
    time(&current_t);
    int log_time = difftime(current_t, begin_t);

    char header[20];
    printl_header(header, log_time, log_level);
    int header_len = strlen(header);
    int cr_cnt = 0;
    for (const char *i = format; *i; ++i) {
        if (*i == '\n' && i[1]) {
            ++cr_cnt;
        }
    }

    char *nformat = malloc((strlen(format) + (cr_cnt + 1) * (header_len + 1) + 2) * sizeof(char));
    memset(nformat, 0, (strlen(format) + (cr_cnt + 1) * (header_len + 1) + 2) * sizeof(char));
    char *nfptr = nformat;
    strcpy(nfptr, header);
    nfptr += header_len;
    
    for (const char *i = format; *i; ++i) {
        *(nfptr++) = *i;
        if (cr_cnt > 0 && *i == '\n') {
            strcpy(nfptr, header);
            nfptr += header_len;
            --cr_cnt;
        }
    }
    *nfptr = '\n';
    while (nfptr > nformat && *nfptr == '\n' && nfptr[-1] == '\n') {
        *(nfptr--) = 0;
    }

    va_list args;
    va_start(args, format);
    int ret = vfprintf(log_file_fd, nformat, args);
    va_end(args);
    fflush(log_file_fd);
    free(nformat);
    return ret;
}
