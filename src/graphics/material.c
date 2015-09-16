#include <graphics/material.h>

material_t default_material;

int init_material(void) {
    default_material.diffuse = default_material.ambient = make_vec3(1, 1, 1);
    default_material.specular = make_vec3(0, 0, 0);
    return 0;
}

material_t make_material(vec3f ambient, vec3f diffuse, vec3f specular, float shininess) {
    material_t ret;
    ret.ambient = ambient;
    ret.diffuse = diffuse;
    ret.specular = specular;
    ret.shininess = shininess;
    return ret;
}
