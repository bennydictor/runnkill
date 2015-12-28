#ifndef GAME_ITEMS_H
#define GAME_ITEMS_H

#include <game/mod_type.h>
#include <game/common.h>
#include <game/material.h>
#include <math/vecmath.h>
#include <string>
#include <iostream>

typedef float* vec3f;

struct item_t {
    mod_t effects;
    int hp, mp, strength, agi, intellect, speed;
    int material_idx;
    float defense_coeff;
    void set_material(vec3f ambient, vec3f diffuse, vec3f specular);
    void in(std::istream& in); 
};


vec3f cpp_make_vec3_0();

vec3f cpp_make_vec3(float x, float y, float z);



#endif // GAME_ITEMS_H
