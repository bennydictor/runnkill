#include <game/items.h>
#include <iostream>
#include <math/vecmath.h>

typedef float* vec3f;

vec3f cpp_make_vec3_0() {
    vec3f res = new float[3];
    return res;
}
vec3f cpp_make_vec3(float x, float y, float z) {
    vec3f res = cpp_make_vec3_0();
    res[0] = x;
    res[1] = y;
    res[2] = z;
    return res;
}
void item_t::set_material(vec3f ambient, vec3f diffuse, vec3f specular) {
    material.ambient = ambient;
    material.diffuse = diffuse;
    material.specular = specular;
    material.shininess = 128;
}

void item_t::in(std::istream& stream) {
    stream >> effects.hp >> effects.mp >> effects.strength >> effects.agility >> effects.intellect >> effects.speed;  
    stream >> defense_coeff;
    float r, g, b;
    vec3f ambient, diffuse, specular;
    ambient = cpp_make_vec3_0();
    diffuse = cpp_make_vec3_0();
    specular = cpp_make_vec3_0();
    stream >> ambient[0] >> ambient[1] >> ambient[2];
    stream >> r >> g >> b;
    diffuse = cpp_make_vec3(r, g, b);
    stream >> r >> g >> b;
    specular = cpp_make_vec3(r, g, b);
    set_material(ambient, diffuse, specular);
}

