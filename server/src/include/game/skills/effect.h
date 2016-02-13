#ifndef GAME_EFFECT_H
#define GAME_EFFECT_H
#ifdef __cplusplus
#include <iostream>
#include <string>
#include <math/vec3.h>
#endif
#include <game/mod_type.h>

class effect {

public:
    std::string name;
    mod_t mods_one_side;
    mod_t mods_two_side;
    float time;
    bool is_stunning, cancel;
    vec3<float> speed;
    int material_idx, owner;
    effect();
    effect(std::string _name);
    mod_t tic(float t);
    void in(std::istream& stream);
    void upgrade(float attack);
};


#endif // GAME_EFFECT_H
