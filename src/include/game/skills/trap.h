#ifndef _GAME_SKILLS_TRAP_H_
#define _GAME_SKILLS_TRAP_H_
#include <math/vec3.h>

struct trap {
    float rad, dmg, time, activate_time;
    bool is_alive;
    vec3<float> centre;
    trap() { };
    trap(vec3<float> _centre, float _rad, float _dmg, float t);
    bool is_intersect(vec3<float> c, float r);
};

#endif // _GAME_SKILLS_TRAP_H_
