#ifndef _GAME_SKILLS_EXPLOSION_H_
#define _GAME_SKILLS_EXPLOSION_H_

#include <math/vec3.h>
#include <vector>
#include <game/skills/effect.h>
#define EXPLOSION_RADIUS .2
#define EXPLOSION_TIME .5

struct explosion {
    int owner; 
    float rad, time, end_time, damage;
    vec3<float> coords;
    std::vector<effect> effects;
    explosion();
    explosion(vec3<float> centre, float end=EXPLOSION_TIME, float rad=EXPLOSION_RADIUS, float dmg=0); 
    float time_c();    
};
#endif //_GAME_SKILLS_EXPLOSION_H_
