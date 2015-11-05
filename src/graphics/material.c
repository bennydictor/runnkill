#include <graphics/material.h>

material_t man_material, default_material, bullet_material, shield_material, explosion_material;

int init_material(void) {
    default_material.diffuse = default_material.ambient = make_vec3(1, 1, 1);
    default_material.specular = make_vec3(0.2, 0.2, 0.2);
    default_material.shininess = 32;
    bullet_material.diffuse = make_vec3(1, 0, 0);
    bullet_material.ambient = make_vec3(0, 1, 0);
    bullet_material.specular = make_vec3(1, 0.5, 0);
    bullet_material.shininess = 32;
    shield_material.diffuse = shield_material.ambient = make_vec3(0, 0.1, 0.5);
    shield_material.specular = make_vec3(0, 0.9, 0.5);
    shield_material.shininess = 32;
    man_material.ambient = make_vec3(0.7, 0.1, 0); man_material.specular = make_vec3(1, .4, 0); man_material.diffuse = make_vec3(1, 0, 0);
    man_material.shininess = 32;
    explosion_material.ambient = make_vec3(1, .5, .3);
    explosion_material.diffuse = explosion_material.specular = make_vec3(0, 0, 0);
    return 0;
}

int init_bullet_material(void) {
    bullet_material.diffuse = make_vec3(1, 0, 0);
    bullet_material.ambient = make_vec3(0, 1, 0);
    bullet_material.specular = make_vec3(1, 0.5, 0);
    bullet_material.shininess = 32;
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
