#ifndef GRAPHICS_DRAW_OBJ
#define GRAPHICS_DRAW_OBJ

#include <math/vecmath.h>
#include <graphics/material.h>


typedef struct {
    mat4f mat_m;
    void *ibo;
    unsigned int mode;
    unsigned int vbo;
    unsigned int count;
    material_t material;
} draw_obj;


#endif // GRAPHICS_DRAW_OBJ
