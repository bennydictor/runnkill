#ifndef GAME_SKILL_TYPE_H
#define GAME_SKILL_TYPE_H

#include <vector>
#include <game/skills/effect.h>
#include <game/mod_type.h>
#include <game/skills/bullet.h>
#include <iostream>


struct skill_t {
    std::string name;       
    std::vector<effect> effects;
    mod_t cost;
    bullet sample;
    char type;
    int animation_idx, material_idx;
    float to_activate, between_activate;
    float busy_time, activate_time, dmg, left_angle, right_angle, height, distance;
    void in_damage(std::istream& stream);
};


#endif // GAME_SKILL_TYPE_H
