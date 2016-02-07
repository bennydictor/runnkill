#include <graphics/material.h>
#include <util/log.h>

material_t materials[MAX_MATERIAL];
int material_count;
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
