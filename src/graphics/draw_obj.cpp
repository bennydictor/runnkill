#include <graphics/draw_obj.h>
#include <vector>
#include <math/vec3.h>
#include <math/constants.h>
#include <math.h>


using namespace std;

draw_sphere_sector::draw_sphere_sector(float radius) {
    mat_m = make_mat4();
    vec3f v = make_vec3(radius, radius, radius);
    scale_mat(v, mat_m);
    free(v);
}

draw_sphere_sector::draw_sphere_sector(vec3<float> pos, float radius) {
    mat_m = make_mat4();
    vec3f v = make_vec3(radius, radius, radius);
    scale_mat(v, mat_m);
    v[0] = pos.x;
    v[1] = pos.y;
    v[2] = pos.z;
    itrans_mat(v, mat_m);
    free(v);
}

unsigned int draw_sphere_sector::vbo() {
    return 1;
}

unsigned int *draw_sphere_sector::ibo() {
    return NULL;
}

draw_box::draw_box(ortohedron bounds) {
    vec3<float> x = bounds.p2 - bounds.p1;
    vec3<float> y = bounds.p3 - bounds.p1;
    vec3<float> z = bounds.p4 - bounds.p1;
    mat_m = make_mat4();
    mat_m[INDEX4(0, 0)] = x.x;
    mat_m[INDEX4(0, 1)] = x.y;
    mat_m[INDEX4(0, 2)] = x.z;
    mat_m[INDEX4(1, 0)] = y.x;
    mat_m[INDEX4(1, 1)] = y.y;
    mat_m[INDEX4(1, 2)] = y.z;
    mat_m[INDEX4(2, 0)] = z.x;
    mat_m[INDEX4(2, 1)] = z.y;
    mat_m[INDEX4(2, 2)] = z.z;
    vec3f v = make_vec3(bounds.p1.x, bounds.p1.y, bounds.p2.z);
    itrans_mat(v, mat_m);
    free(v);
}

unsigned int draw_box::vbo() {
    return 2;
}

unsigned int *draw_box::ibo() {
    return NULL;
}
