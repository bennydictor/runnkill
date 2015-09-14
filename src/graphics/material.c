#include <graphics/material.h>

material_t make_material(vec3f ambient, vec3f diffuse, vec3f specular, float shininess) {
    material_t ret;
    ret.ambient = ambient;
    ret.diffuse = diffuse;
    ret.specular = specular;
    ret.shininess = shininess;
    return ret;
}
