#ifndef MAN
#define MAN

#include <common.h>
#include <items.h>
#include <vector>
#include <string>
#include <body_part.h>
#include <effect.h>
#include <mod_type.h>
#include <skill_type.h>
#include <vec2.h>

struct man {
    std::string name;
    int cls;
    int level, exp;
    float def_mod, atk_mod;
    double time;
    vec2<float> coords, speed;
    std::vector<item_t> bag;
    std::vector<body_part> body_parts;
    std::vector<effect> effects;
    std::vector<skill_t> skills;
    int hp, mn, agility, strength, intellect, abs_speed;
    man(std::string _name, int cl);
    void move(float curr_time);
    void set_speed(vec2<float> spd);
    void get_effect(mod_t res);
};
#endif // MAN
