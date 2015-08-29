#include <man.h>

using namespace std;
man::man(string _name, int cl) {
    name = _name;
    cls = cl;
    def_mod = atk_mod = 1; 
    for (size_t i = 0; i < BP_AMOUNT; i++) {
        body_parts.push_back(bp_names[i]);
    }
    init_values(hp, mn, agility, strength, intellect, speed, cl);
    exp = level = 0;
}

man::set_speed(vec2 spd) {
    speed = spd;
}

man::get_effect(mod_t res) {
    hp += res.hp;
    mn += res.mn;
    agility *= res.agi;
    strength *= res.str;
    intellect *= res.intellect;
    speed *= res.speed;
    def_mod *= res.def_mod;
    atk_mod *= res.atk_mod;
}

man::move(float curr_time) {
    coords += speed * (curr_time - time);
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


