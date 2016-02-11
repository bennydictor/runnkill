#ifndef GAME_MAN_H
#define GAME_MAN_H
#define MAN_RAD .4

#include <stdlib.h>
#ifdef __cplusplus
#include <iostream>
#include <string>
#include <vector>
#include <map>
#endif
//#include <game/armour.h>
#include <game/skills/aura.h>
#include <game/body_part.h>
#include <game/common.h>
#include <game/skills/effect.h>
#include <game/items.h>
#include <game/mod_type.h>
#include <game/skills/skill_type.h>
#include <game/message.h>
#include <math/vec2.h>
#include <math/vec3.h>

extern int GAME_MAX_MAN_IDX;

struct man {
    std::string name;
    int cls, curr_skill;
    float def_mod, atk_mod, busy, attack_rad, sum_damage;
    bool can_die, have_shield, is_running, touch_ground, need_to_cast;
    char buff[2048];
    mod_t recovery;
    std::vector<item_t*> bag;
    std::vector<body_part> body_parts;
    std::vector<effect> effects;
    std::vector<abstract_skill_t* > skills;
    std::vector<message> messages;
    //armour* weapon;
    aura* my_aura;
    std::map<int, float> damagers, healers;
    vec3<float> coords, speed, orientation;
    float hp, mp;
    float max_hp, max_mp, agility, strength, intellect, abs_speed, jump_high;
    int exp, level, level_exp, number;
    man();
    man(std::string _name, int cl);
    man(std::istream& file);
    vec3<float> in_time(float time);
    void move(float time);
    void add_effect(effect a);
    void set_speed(vec3<float> spd);
    void set_orientation(vec3<float> orient);
    void get_effect_1(mod_t res);
    void get_effect_2(mod_t res);
    void fortify(int idx);
    void out(std::ostream& stream);
    void write_info(std::ostream& file); 
    void run(bool must_run);
    void get_exp(int e);
    void respawn();
    void put_on(item_t* item, int idx);
    void new_message(char* s, float t);
    char* get_text();
    bool take_damage(float dmg, int owner);
};

int count_attack(man z);
#endif // GAME_MAN_H
