#include <vector>
#include <effect.h>
#include <mod_type.h>
#include <bullet.h>
#include <iostream>
#ifndef SKILL_TYPE
#define SKILL_TYPE


struct skill_t {
    std::string name;       
    std::vector<effect> effects;
    mod_t cost;
    bullet sample;
    bool is_range;
    int dmg, u_l, u_r, d_l, d_r;
    void in_damage(std::istream& stream);
};


#endif // SKILL_TYPE
