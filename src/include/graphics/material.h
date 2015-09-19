#ifndef GRAPHICS_MATERIAL_H
#define GRAPHICS_MATERIAL_H

#include <math/vecmath.h>


typedef struct {
    vec3f ambient;
    vec3f diffuse;
    vec3f specular;
    float shininess;
} material_t;

extern material_t default_material;

int init_material(void);
material_t make_material(vec3f ambient, vec3f diffuse, vec3f specular, float shininess);


#endif // GRAPHICS_MATERIAL_H 
