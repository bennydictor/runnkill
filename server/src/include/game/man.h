#ifndef GAME_MAN_H
#define GAME_MAN_H
#define MAN_RAD .4

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

//#include <game/armour.h>
#include <game/body_part.h>
#include <game/common.h>
#include <game/skills/effect.h>
#include <game/items.h>
#include <game/mod_type.h>
#include <game/skills/skill_type.h>

#include <math/vec2.h>
#include <math/vec3.h>

struct man {
    std::string name;
    int cls, curr_skill;
    float def_mod, atk_mod, busy, attack_rad;
    bool can_die, have_shield, is_running, touch_ground, need_to_cast;
    mod_t recovery;
    std::vector<item_t*> bag;
    std::vector<body_part> body_parts;
    std::vector<effect> effects;
    std::vector<skill_t> skills;
    //armour* weapon;
    vec3<float> coords, speed, orientation;
    float hp, mp;
    float max_hp, max_mp, agility, strength, intellect, abs_speed, jump_high;
    int level, exp, number;
    man();
    man(std::string _name, int cl);
    vec3<float> in_time(float time);
    void move(float time);
    void add_effect(effect a);
    void set_speed(vec3<float> spd);
    void set_orientation(vec3<float> orient);
    void get_effect_1(mod_t res);
    void get_effect_2(mod_t res);
    void fortify(int idx);
    void out(std::ostream& stream);
    void run();
    void put_on(item_t* item, int idx);
    bool take_damage(int dmg);
};

int count_attack(man z);
#endif // GAME_MAN_H
