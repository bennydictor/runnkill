#ifndef GAME_SKILLS_AURA_H
#define GAME_SKILLS_AURA_H
#include <game/skills/effect.h>
#include <vector>
struct aura {
    float time, distance;
    float can_use, tic;
    int owner;
    std::vector<effect> effects;
    aura(float t, float dt, float d, std::vector<effect>& e);
    void set_owner(int o);
};

#endif
