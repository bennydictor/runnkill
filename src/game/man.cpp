#include <game/man.h>
#include <game/common.h>
#include <game/animation.h>
#include <cassert>
#include <cstdlib>
using namespace std;
man::man() {
    cls = 0;
    def_mod = atk_mod = 1; 
    can_die = true;
    have_shield = false;
    touch_ground = false;
//    weapon = NULL;
    orientation = vec3<float>(1, 0, 0);
    for (size_t i = 0; i < BP_AMOUNT; i++) {
        body_parts.push_back(body_part(bp_names[i], bp_init_mods[i]));
    }
    init_values(hp, mp, agility, strength, intellect, abs_speed, jump_high, attack_rad, cls);
    max_hp = hp;
    max_mp = mp;
    curr_skill = -1;

    exp = level = 0;
    number = rand();
    recovery.hp = 0;
    recovery.mp = 3;
}
man::man(string _name, int cl) {
    name = _name;
    cls = cl;
    def_mod = atk_mod = 1; 
    can_die = true;
    have_shield = false;
    touch_ground = false;
//    weapon = NULL;
    orientation = vec3<float>(1, 0, 0);
    for (size_t i = 0; i < BP_AMOUNT; i++) {
        body_parts.push_back(body_part(bp_names[i], bp_init_mods[i]));
    }
    init_values(hp, mp, agility, strength, intellect, abs_speed, jump_high, attack_rad, cl);
    busy = exp = level = 0;
    speed = vec3<float>(abs_speed, 0, 0);
    number = rand();
    max_hp = hp;
    max_mp = mp;
    curr_skill = -1;
    recovery.hp = 0;
    recovery.mp = 3;
}

void man::set_speed(vec3<float> spd) {
    speed = spd;
}

void man::set_orientation(vec3<float> orient) {
    orientation = orient;
    orientation.resize(1);
}
void man::get_effect_1(mod_t res) {
    hp += res.hp;
    mp += res.mp;
}

void man::get_effect_2(mod_t res) {
    if (res.agility > EPS) 
        agility *= res.agility;
    if (res.strength >  EPS) 
        strength *= res.strength;
    if (res.intellect > EPS) 
        intellect *= res.intellect;
    if (res.speed > EPS) 
        abs_speed *= res.speed;
    if (res.def_mod > EPS) 
        def_mod *= res.def_mod;
    if (res.atk_mod > EPS) 
        atk_mod *= res.atk_mod;
    cout << '!' << abs_speed << endl; 
}
void man::add_effect(effect a) {
    effects.push_back(a);
    get_effect_2(a.mods_two_side);
}
vec3<float> man::in_time(float time) {
    int amount_of_f = 0;
    for (int j = 1; j < BP_AMOUNT; j++) {
        if (body_parts[j].is_fortified) {
            amount_of_f++;
        }
    }
    if (have_shield) {
        amount_of_f--;
    }
    vec3<float> speed_tmp = speed;
    speed_tmp.y = 0;
    vec3<float> ret = coords + (float)((time * max(0.1f, (1 - (float)0.2 * amount_of_f))) * (is_running ? (have_shield ? 1.6 : 2) : 1)) * speed_tmp;
    ret.y += time * speed.y;
    char a;
    if (!(abs(speed.x) >= 0))
        cin >> a;
    //cout << ret << ' ' << speed << endl;
    return ret;
}

void man::move(float time) {
    if (time <= 0)
        return;
    assert(time > 0);
    vector<effect> new_effects;
    for (size_t i = 0; i < effects.size(); i++)
    {
        mod_t res = effects[i].tic(time);
        get_effect_1(res);
        if (effects[i].time >= 0 and effects[i].time - time <= 0)
        {
            get_effect_2(effects[i].mods_two_side * -1);
        }
        effects[i].time -= time;
        if (effects[i].time > -EPS) {
            new_effects.push_back(effects[i]);
        }
    }
    effects = new_effects;
    for (int i = 0; i < (int)skills.size(); i++) {
        skills[i].to_activate -= time; 
    }
    //cout << busy << endl;
    busy = max((float)-EPS, busy - time);
    if (busy < 0) {
        curr_skill = -1;
    }
    hp += (recovery.hp * time);
    hp = min(hp, (float)max_hp);
    mp += (recovery.mp * time);
    mp = min(mp, (float)max_mp);
}

bool man::take_damage(int dmg) {
    cout << dmg << endl;
    if (can_die) {
        hp = max(hp - dmg, 0.0f);
    } else {
        hp = max(hp - dmg, 1.0f);
    }
    return (hp < EPS);
    
}
int count_attack(man z) {
    return atk_strength_mods[z.cls] * z.strength + atk_agi_mods[z.cls] * z.agility + atk_int_mods[z.cls] * z.intellect;
}

void man::fortify(int idx) {
    body_parts[idx].is_fortified ^= 1;
}

void man::out(ostream& stream) {
    stream << name << ' ' << (cls == 0 ? "Warrior" : ((cls == 1) ? "Archer" : "Mage")) << ", " << level << " lvl, " << endl;
    stream << hp << ' ' << mp << endl;
    stream << "My attack = " << count_attack(*this) << endl;
    stream << "I`m in " << coords << endl;
}

void man::run() {
    is_running = true;
    for (int i = 0; i < (int) body_parts.size(); i++)
    {
        body_parts[i].is_fortified = false;
    }
    if (have_shield)
    {
        body_parts[LEFT_FRONT_UP].is_fortified = true;
    }
}

void man::put_on(item_t* item, int idx) {
    item_t* wore = body_parts[idx].put_on(item);
    if (wore)
    {
        get_effect_2(wore->effects * -1);
        max_hp -= wore->hp;
        max_mp -= wore->mp;
       
        bag.push_back(wore);
    }
    get_effect_2(item->effects);
    max_hp += item->hp;
    max_mp += item->mp;
}
