#ifndef EFFECT_H
#define EFFECT_H

#include <iostream>
#include <string>
#include <game/mod_type.h>

class effect {

public:
    std::string name;
    mod_t mods_one_side;
    mod_t mods_two_side;
    float time;
    effect(std::string _name) : name(_name) {}
    mod_t tic(float t);
    void in(std::istream& stream);
};


#endif // EFFECT_H
