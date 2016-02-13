#ifndef GAME_SKILL_TYPE_H
#define GAME_SKILL_TYPE_H

#include <vector>
#include <game/skills/effect.h>
#include <game/mod_type.h>
#include <game/skills/bullet.h>
#include <iostream>


struct abstract_skill_t {
    std::string name;       
    std::vector<effect> effects;
    mod_t cost;
    char type;
    int animation_idx, material_idx;
    float to_activate_skill, between_activate_skill, activate_time; // activate_time is time before the end of the animation, when this skill should be activated
    abstract_skill_t() = default;
    ~abstract_skill_t() = default;
    void abstract_input(std::istream& stream);
    virtual void in_damage(std::istream& stream) = 0;
};

struct meelee_skill_t : public abstract_skill_t {
    float dmg, left_angle, right_angle, height, distance;
    virtual void in_damage(std::istream& stream);
};

struct range_skill_t : public abstract_skill_t {
    bullet sample;
    float dmg;
    virtual void in_damage(std::istream& stream);
};

struct trap_skill_t : public abstract_skill_t {
    float rad, busy_time, dmg; // busy_time is time between the activation of the skill and the beginning of the trap's work
    virtual void in_damage(std::istream& stream);
};

struct aura_skill_t : public abstract_skill_t {
    float duration, rad, tic;
    std::vector<effect> my_effects;
    virtual void in_damage(std::istream& stream);
};

abstract_skill_t* new_skill(abstract_skill_t* old); 
#endif // GAME_SKILL_TYPE_H
