#ifndef GRAPHICS_DRAW_OBJ_T_H
#define GRAPHICS_DRAW_OBJ_T_H

#include <graphics/material.h>


typedef struct {
    mat4f mat_m;
    void *ibo;
    unsigned int mode;
    unsigned int vbo;
    unsigned int count;
    material_t material;
} draw_obj_t;

#endif // GRAPHICS_DRAW_OBJ_T_H
