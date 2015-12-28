#include <vector>
#include <game/skills/effect.h>
#include <game/skills/skill_type.h>
#include <iostream>
using namespace std;

void skill_t::in_damage(istream& stream) {
    stream >> cost.hp >> cost.mp >> between_activate;
    stream >> dmg;
    if (type == 'R') {
        sample.in(stream);
        sample.damage = dmg;
    } else if (type == 'M') {
        stream >> left_angle >> right_angle >> height >> distance;
    } else if (type == 'T') {
        stream >> distance >> busy_time >> material_idx; 
    }
    int amount_of_effects;
    stream >> amount_of_effects;
    string eff_name;
    for (int i = 0; i < amount_of_effects; i++) {
        stream >> eff_name;
        effects.push_back(effect(eff_name));
        effects[i].in(stream);
    }
    if (type == 'R') {
        sample.effects = effects;
    }
    stream >> animation_idx >> activate_time;
    to_activate = 0;
}

