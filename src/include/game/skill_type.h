#ifndef GAME_SKILL_TYPE_H
#define GAME_SKILL_TYPE_H

#include <vector>
#include <game/effect.h>
#include <game/mod_type.h>
#include <game/bullet.h>
#include <iostream>


struct skill_t {
    std::string name;       
    std::vector<effect> effects;
    mod_t cost;
    bullet sample;
    bool is_range;
    int animation_idx;
    float busy_time, activate_time, dmg, left_angle, right_angle, height, dir;
    void in_damage(std::istream& stream);
};


#endif // GAME_SKILL_TYPE_H
