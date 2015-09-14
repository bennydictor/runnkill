#ifndef GRAPHICS_DRAW_OBJ
#define GRAPHICS_DRAW_OBJ

#include <vector>
#include <math/vecmath.h>
#include <math/vec3.h>
#include <math/ortohedron.h>

struct draw_obj {
    mat4f mat_m;
    virtual unsigned int vbo() { return 0; };
    virtual unsigned int ibo_size() { return 0; };
    virtual unsigned int *ibo() { return NULL; };
};

struct draw_sphere_sector : public draw_obj {
    virtual unsigned int vbo();
    virtual unsigned int *ibo();
    draw_sphere_sector() {}
    draw_sphere_sector(float radius);
    draw_sphere_sector(vec3<float> pos, float radius);
};

struct draw_box : public draw_obj {
    virtual unsigned int vbo();
    virtual unsigned int *ibo();
    draw_box() {}
    draw_box(ortohedron bounds);
};

#endif // GRAPHICS_DRAW_OBJ
