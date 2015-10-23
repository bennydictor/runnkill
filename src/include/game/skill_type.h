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
    float busy_time, dmg, u_l, u_r, d_l, d_r;
    void in_damage(std::istream& stream);
};


#endif // GAME_SKILL_TYPE_H
