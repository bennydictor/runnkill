#include <math/ortohedron.h>
#include <math/vecmath.h>

ortohedron::ortohedron(vec3<float> a, vec3<float> b, vec3<float> c, vec3<float> d) {
    p1 = a;
    p2 = b;
    p3 = c;
    p4 = d;
    vec_table[0] = make_vec3(p1.x, p1.y, p1.z);
    vec_table[1] = make_vec3(p2.x, p2.y, p2.z);
    vec_table[2] = make_vec3(p3.x, p3.y, p3.z);
    vec_table[3] = make_vec3(p4.x, p4.y, p4.z);
}

ortohedron::operator vec3f *() {
    return vec_table;
}

ortohedron::~ortohedron() {
    free(vec_table[0]);
    free(vec_table[1]);
    free(vec_table[2]);
    free(vec_table[3]);
}
