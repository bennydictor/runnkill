#ifndef GAME_SKILLS_BULLET_H
#define GAME_SKILLS_BULLET_H

#include <iostream>
#include <game/skills/effect.h>
#include <vector>
#include <string>
#include <math/vec3.h>

#define GRAVITATION 9


struct bullet {
    vec3<float> speed, coords;
    std::string name;
    std::vector<effect> effects;
    float damage;
    float rad, exp_rad;
    int bullet_m_idx, explosion_m_idx;
    bullet();
    void in(std::istream& stream);
    void upgrade(float attack);
    vec3<float> in_time(float time);
    int owner;
};


#endif // GAME_BULLET_H
