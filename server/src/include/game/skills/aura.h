#ifndef GAME_SKILLS_AURA_H
#define GAME_SKILLS_AURA_H
#include <game/skills/effect.h>
#include <vector>
struct aura {
    float time, distance;
    float can_use, tic;
    int owner, circle_material_idx;
    std::vector<effect> effects, my_effects;
    aura(float t, float dt, float d, int material_idx, std::vector<effect> e, std::vector<effect> my_e);
    void set_owner(int o);
};

#endif
