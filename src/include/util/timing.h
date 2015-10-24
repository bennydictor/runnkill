#ifndef UTIL_TIMING_H
#define UTIL_TIMING_H

#ifdef __cplusplus
extern "C" {
#endif

float prev_time;
float last_point;
unsigned int fps;

float delta(void);

#ifdef __cplusplus
}
#endif

#endif /// UTIL_TIMING_H
