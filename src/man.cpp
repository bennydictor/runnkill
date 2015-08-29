#include <man.h>
#include <common.h>

using namespace std;
man::man(string _name, int cl) {
    name = _name;
    cls = cl;
    def_mod = atk_mod = 1; 
    for (size_t i = 0; i < BP_AMOUNT; i++) {
        body_parts.push_back(body_part(bp_names[i], bp_init_mods[i]));
    }
    init_values(hp, mn, agility, strength, intellect, abs_speed, cl);
    exp = level = 0;
}

void man::set_speed(vec2<float> spd) {
    speed = spd;
}

void man::get_effect(mod_t res) {
    hp += res.hp;
    mn += res.mn;
    agility *= res.agility;
    strength *= res.strength;
    intellect *= res.intellect;
    speed = res.speed * speed;
    def_mod *= res.def_mod;
    atk_mod *= res.atk_mod;
}

void man::move(float curr_time) {
    coords = coords + (float)(curr_time - time) * speed ;
    for (size_t i = 0; i < effects.size(); i++)
    {
        mod_t res = effects[i].tic(curr_time - time);
        this->get_effect(res);
        if (effects[i].time <= 0)
        {
            this->get_effect(effects[i].mods_two_side * -1);
        }
    }
    time = curr_time;
}


