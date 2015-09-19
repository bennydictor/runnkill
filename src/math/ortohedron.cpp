#include <math/ortohedron.h>

ortohedron::ortohedron(vec3<float> a, vec3<float> b, vec3<float> c, vec3<float> d) {
    p1 = a;
    p2 = b;
    p3 = c;
    p4 = d;
    vec_table[0] = (vec3f) p1;
    vec_table[1] = (vec3f) p2;
    vec_table[2] = (vec3f) p3;
    vec_table[2] = (vec3f) p4;
}

ortohedron::operator vec3f *() {
    return vec_table;
}
