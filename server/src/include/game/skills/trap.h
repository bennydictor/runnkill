#ifndef _GAME_SKILLS_TRAP_H_
#define _GAME_SKILLS_TRAP_H_
#include <math/vec3.h>
#include <vector>
#include <game/skills/effect.h>

struct trap {
    float rad, dmg, time, activate_time;
    int material_idx;
    bool is_alive;
    vec3<float> centre;
    std::vector<effect> effects;
    trap() { };
    trap(vec3<float> _centre, float _rad, float _dmg, float t, int i);
    bool is_intersect(vec3<float> c, float r);
};

#endif // _GAME_SKILLS_TRAP_H_
