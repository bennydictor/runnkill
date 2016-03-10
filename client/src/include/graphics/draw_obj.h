#ifndef GRAPHICS_DRAW_OBJ
#define GRAPHICS_DRAW_OBJ

#include <math/vecmath.h>
#include <graphics/material.h>

// Defines DRAW_SPHERE etc are in src/include/world.h

typedef struct {
    mat4f mat_m;
    void *ibo;
    unsigned int mode;
    unsigned int vbo;
    unsigned int count;
    material_t material;
    char free_mat_m;
    char free_ibo;
} draw_obj;

void free_draw_obj(draw_obj obj);


#endif // GRAPHICS_DRAW_OBJ
