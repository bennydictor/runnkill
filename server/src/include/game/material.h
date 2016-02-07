#ifndef GRAPHICS_MATERIAL_H
#define GRAPHICS_MATERIAL_H

#include <math/vecmath.h>

#define MAX_MATERIAL 256

#ifdef __cplusplus
extern "C" {
#endif

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
extern material_t default_material;
extern material_t bullet_material;
extern material_t trap_material;
extern material_t shield_material;
extern material_t man_material;
extern material_t explosion_material;

int init_material(void);

#ifdef __cplusplus
}
#endif

#endif // GRAPHICS_MATERIAL_H 
