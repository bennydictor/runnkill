#include <game/man.h>
#include <game/common.h>

using namespace std;
man::man() {
    cls = 0;
    def_mod = atk_mod = 1; 
    can_die = true;
    have_shield = false;
    orientation = vec3<float>(1, 0, 0);
    for (size_t i = 0; i < BP_AMOUNT; i++) {
        body_parts.push_back(body_part(bp_names[i], bp_init_mods[i]));
    }
    init_values(hp, mp, agility, strength, intellect, abs_speed, cls);
    exp = level = 0;
}
man::man(string _name, int cl) {
    name = _name;
    cls = cl;
    def_mod = atk_mod = 1; 
    can_die = true;
    have_shield = false;
    orientation = vec3<float>(1, 0, 0);
    for (size_t i = 0; i < BP_AMOUNT; i++) {
        body_parts.push_back(body_part(bp_names[i], bp_init_mods[i]));
    }
    init_values(hp, mp, agility, strength, intellect, abs_speed, cl);
    exp = level = 0;
}

void man::set_speed(vec3<float> spd) {
    speed = spd;
}

void man::set_orientation(vec3<float> orient) {
    orientation = orient;
    orientation.resize(1);
}
void man::get_effect(mod_t res) {
    hp += res.hp;
    mp += res.mp;
    agility *= res.agility;
    strength *= res.strength;
    intellect *= res.intellect;
    speed = res.speed * speed;
    def_mod *= res.def_mod;
    atk_mod *= res.atk_mod;
}

vec3<float> man::in_time(float time) {
    int amount_of_f = 0;
    for (int j = 0; j < BP_AMOUNT; j++) {
        if (body_parts[j].is_fortified) {
            amount_of_f++;
        }
    }
    if (have_shield) {
        amount_of_f--;
    }
    return coords + (float)((time) * (1 - (float)0.2 * amount_of_f)) * speed;
}

void man::move(float time) {
    for (size_t i = 0; i < effects.size(); i++)
    {
        mod_t res = effects[i].tic(time);
        this->get_effect(res);
        if (effects[i].time <= 0)
        {
            this->get_effect(effects[i].mods_two_side * -1);
        }
    }
}

bool man::take_damage(int dmg) {
    if (can_die) {
        hp = max(hp - dmg, 0);
    } else {
        hp = max(hp - dmg, 1);
    }
    return (hp == 0);
    
}
int count_attack(man z) {
    return atk_strength_mods[z.cls] * z.strength + atk_agi_mods[z.cls] * z.agility + atk_int_mods[z.cls] * z.intellect;
}

void man::fortify(int idx) {
    if (idx == RIGHT_UP or body_parts[(idx - 2 + BP_AMOUNT) % BP_AMOUNT].is_fortified or 
                           body_parts[(idx + 2 + BP_AMOUNT) % BP_AMOUNT].is_fortified) {
        body_parts[idx].is_fortified = true;
    }
}

void man::out(ostream& stream) {
    stream << name << ' ' << (cls == 0 ? "Warrior" : ((cls == 1) ? "Archer" : "Mage")) << ", " << level << " lvl, " << endl;
    stream << hp << ' ' << mp << endl;
    stream << "My attack = " << count_attack(*this) << endl;
    stream << "I`m in " << coords << endl;
}


void man::put_on(item_t* item, segment) {
    item_t* wore = body_parts[segment].put_on(item);
    if (wore)
        bag.push_back(wore);
    return;
}
