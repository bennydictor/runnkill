#ifndef GRAPHICS_MATERIAL_H
#define GRAPHICS_MATERIAL_H

#include <math/vecmath.h>

#define MAX_MATERIAL 256


typedef struct {
    vec3f ambient;
    vec3f diffuse;
    vec3f specular;
    float shininess;
    float alpha;
    short int id;
} material_t;

material_t make_material(vec3f ambient, vec3f diffuse, vec3f specular, float shininess, float alpha);
extern material_t materials[MAX_MATERIAL];
extern int material_count;


#endif // GRAPHICS_MATERIAL_H 
