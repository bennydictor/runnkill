#include <util/timing.h>
#include <sys/time.h>
#include <util/log.h>
#include <stdlib.h>


double prev_time, last_point;
unsigned int fps, cur_fps;

double delta(void) {
    ++cur_fps;
    struct timeval ct;
    gettimeofday(&ct, NULL);
    double cur_time = 1.0 * ct.tv_sec + 1.0e-6 * ct.tv_usec;
    while (cur_time - prev_time < 1.0 / 1024) {
        gettimeofday(&ct, NULL);
        cur_time = 1.0 * ct.tv_sec + 1.0e-6 * ct.tv_usec;
    }
    if (cur_time - last_point >= 1) {
        fps = cur_fps;
        cur_fps = 0;
        last_point = cur_time;
    }
    double d = cur_time - prev_time;
    prev_time = cur_time;
    return d;
}
