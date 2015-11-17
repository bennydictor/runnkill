#ifndef GAME_ARMOUR_H_
#define GAME_ARMOUR_H_

#include <iostream>
#include <vector>

#include <math/vec3.h>
#include <math/geom.h>

#include <game/util/event.h>

#include <graphics/draw_obj.h>

#include <graphics/material.h>
#include <graphics/vertex3d.h>

struct armour {

    unsigned int* ibo_data;
    std::vector<vec3<float> > points;
    std::vector<vec3<float> > normal;
    int p1_idx, p2_idx;
    int type;
    float scale;
    int count;
    unsigned int vbo_buffer;
    material_t _material;
    armour();
    void get(vec3<float> p1, vec3<float> p2, float angle, std::vector<vec3<float> >& vbo);
    void in(std::istream& stream); 
    draw_obj give_me_points(event E, event other=event());

};

#endif //GAME_ARMOUR_H_
