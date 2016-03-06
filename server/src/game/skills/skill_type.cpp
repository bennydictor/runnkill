#include <vector>
#include <game/skills/effect.h>
#include <game/skills/skill_type.h>
#include <util/logstream.h>

using namespace std;

void abstract_skill_t::abstract_input(istream& stream) {
    stream >> cost.hp >> cost.mp >> between_activate_skill;
    stream >> animation_idx >> activate_time;
    int amount_of_effects;
    stream >> amount_of_effects;
    cout << "1111" << type << amount_of_effects << endl;
    string eff_name;
    for (int i = 0; i < amount_of_effects; i++) {
        stream >> eff_name;
        effects.push_back(effect(eff_name));
        effects[i].in(stream);
    }
    to_activate_skill = 0;
}

void melee_skill_t::in_damage(istream& stream) {
    abstract_input(stream);
    type = 'M';
    stream >> dmg;
    stream >> left_angle >> right_angle >> height >> distance;
}

void range_skill_t::in_damage(istream& stream) {
    abstract_input(stream);
    type = 'R';
    stream >> dmg;
    sample.in(stream);
    sample.damage = dmg;
    sample.effects = effects;
    to_activate_skill = 0;
}

void trap_skill_t::in_damage(istream& stream) {
    abstract_input(stream);
    type = 'T';
    stream >> dmg;
    stream >> rad >> busy_time >> material_idx; 
}

void aura_skill_t::in_damage(istream& stream) {
    abstract_input(stream);
    int amount_of_effects;
    stream >> amount_of_effects;

    string eff_name;
    for (int i = 0; i < amount_of_effects; i++) {
        stream >> eff_name;
        my_effects.push_back(effect(eff_name));
        my_effects.back().in(stream);
    }
    type = 'A';
    stream >> duration >> rad >> tic;
    stream >> circle_material_idx;
}

abstract_skill_t* new_skill(abstract_skill_t* old) {
    abstract_skill_t* ret = NULL;
    if (old->type == 'M') {
        ret = new melee_skill_t(*(melee_skill_t* )old);

    } else if (old->type == 'R') {
        ret = new range_skill_t(*(range_skill_t* )old);
    } else if (old->type == 'A') {
        ret = new aura_skill_t(*(aura_skill_t *) old);
    } else if (old->type == 'T') {
        ret = new trap_skill_t(*(trap_skill_t *) old);
    } else {
        return NULL;
    }
    return ret;
    //ret->
}

