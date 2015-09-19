#ifndef GAME_MAN_H
#define GAME_MAN_H
#define MAN_RAD 1

#include <game/common.h>
#include <game/items.h>
#include <vector>
#include <string>
#include <game/body_part.h>
#include <game/effect.h>
#include <game/mod_type.h>
#include <game/skill_type.h>
#include <math/vec2.h>
#include <math/vec3.h>
#include <iostream>

struct man {
    std::string name;
    int cls;
    int level, exp;
    float def_mod, atk_mod;
    bool can_die, have_shield, is_running;
    vec3<float> coords, speed, orientation;
    std::vector<item_t*> bag;
    std::vector<body_part> body_parts;
    std::vector<effect> effects;
    std::vector<skill_t> skills;
    int hp, mp, agility, strength, intellect, abs_speed;
    man();
    man(std::string _name, int cl);
    vec3<float> in_time(float time);
    void move(float time);
    void set_speed(vec3<float> spd);
    void set_orientation(vec3<float> orient);
    void get_effect(mod_t res);
    void fortify(int idx);
    void out(std::ostream& stream);
    void run();
    bool take_damage(int dmg);
};

int count_attack(man z);
#endif // GAME_MAN_H
