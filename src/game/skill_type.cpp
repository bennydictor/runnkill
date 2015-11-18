#include <vector>
#include <game/effect.h>
#include <game/skill_type.h>
#include <iostream>
using namespace std;

void skill_t::in_damage(istream& stream) {
    stream >> cost.hp >> cost.mp;
    stream >> dmg;
    if (is_range) {
        sample.in(stream);
        sample.damage = dmg;
    } else {
        stream >> left_angle >> right_angle >> height >> dir;
    }
    int amount_of_effects;
    stream >> amount_of_effects;
    string eff_name;
    for (int i = 0; i < amount_of_effects; i++) {
        stream >> eff_name;
        effects.push_back(effect(eff_name));
        effects[i].in(stream);
    }
    if (is_range) {
        sample.effects = effects;
    }
    stream >> animation_idx >> activate_time;
}

