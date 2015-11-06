#ifndef GAME_ITEMS_H
#define GAME_ITEMS_H

#include <game/mod_type.h>
#include <game/common.h>
#include <graphics/material.h>
#include <math/vecmath.h>
#include <string>
#include <iostream>

struct item_t {
    mod_t effects;
    int strength, agi, intellect, speed;
    material_t material;
    float defense_coeff;
    void set_material(vec3f ambient, vec3f diffuse, vec3f specular);
    void in(std::istream& in); 
};

#endif // GAME_ITEMS_H
