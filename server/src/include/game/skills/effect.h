#ifndef GAME_EFFECT_H
#define GAME_EFFECT_H
#ifdef __cplusplus
#include <iostream>
#include <string>
#endif
#include <game/mod_type.h>

class effect {

public:
    std::string name;
    mod_t mods_one_side;
    mod_t mods_two_side;
    float time;
    int material_idx, owner;
    effect();
    effect(std::string _name);
    mod_t tic(float t);
    void in(std::istream& stream);
};


#endif // GAME_EFFECT_H
