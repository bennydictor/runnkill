#ifndef _GAME_UTIL_EVENT_H_
#define _GAME_UTIL_EVENT_H_

#include <math/vec3.h>
#include <iostream>

struct event {

    vec3<float> point1, point2;
    float angle;
    float dt;
    event();
    event(vec3<float> p1, vec3<float> p2, float a);
    void in(std::istream& stream);
    float get_dist();
};

#endif //_GAME_UTIL_EVENT_H_
