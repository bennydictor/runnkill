#ifndef MAN
#define MAN
#define MAN_RAD 1

#include <common.h>
#include <items.h>
#include <vector>
#include <string>
#include <body_part.h>
#include <effect.h>
#include <mod_type.h>
#include <skill_type.h>
#include <vec2.h>
#include <vec3.h>
#include <iostream>

struct man {
    std::string name;
    int cls;
    int level, exp;
    float def_mod, atk_mod;
    bool can_die, have_shield;
    vec3<float> coords, speed, orientation;
    std::vector<item_t> bag;
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
    bool take_damage(int dmg);
};

int count_attack(man z);
#endif // MAN
