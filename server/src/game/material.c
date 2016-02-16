#include <game/material.h>
#include <util/log.h>

material_t materials[MAX_MATERIAL];
int material_count;
material_t man_material, default_material, bullet_material, shield_material, explosion_material;
material_t make_material(vec3f ambient, vec3f diffuse, vec3f specular, float shininess, float alpha) {
    material_t ret;
    if (material_count == MAX_MATERIAL) {
        printl(LOG_W, "Error while making material: Material limit reached");
    }
    ret.ambient = ambient;
    ret.diffuse = diffuse;
    ret.specular = specular;
    ret.shininess = shininess;
    ret.alpha = alpha;
    ret.id = material_count++;
    materials[ret.id] = ret;
    return ret;
}

int init_material(void) {
    default_material = make_material(make_vec3(1, 1, 1), make_vec3(1, 1, 1), make_vec3(.2, .2, .2), 32, 1);
    bullet_material = make_material(make_vec3(1, 0, 0), make_vec3(0, 1, 0), make_vec3(1, .5, 0), 32, 1);
    shield_material = make_material(make_vec3(0, .1, .5), make_vec3(0, .1, .5), make_vec3(0, .9, .5), 32, 1);
    man_material = make_material(make_vec3(.6, .1, 0), make_vec3(1, 0, 0), make_vec3(1, 1, 1), 32, 1);
    explosion_material = make_material(make_vec3(1, .5, .3), make_vec3(0, 0, 0), make_vec3(0, 0, 0), 32, 1);
    return 0;
}
