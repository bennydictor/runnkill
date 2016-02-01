#include <graphics/objects/rect.h>
#include <graphics/gl.h>
#include <string.h>


unsigned int rect_vbo;
vertex3d rect_vbo_data[4] = {
    {{0, 0, 0}, {0, 0}, {0, 0, 1}},
    {{1, 0, 0}, {1, 0}, {0, 0, 1}},
    {{1, 1, 0}, {1, 1}, {0, 0, 1}},
    {{0, 1, 0}, {0, 1}, {0, 0, 1}},
};

void init_rect_object(void) {
    glGenBuffers(1, &rect_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, rect_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vbo_data), rect_vbo_data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void free_rect_object(void) {
    glDeleteBuffers(1, &rect_vbo);
}

draw_obj make_draw_rect(int x1, int y1, int x2, int y2, material_t _material) {
    draw_obj ret;
    ret.mat_m = make_mat4();
    id_mat4(ret.mat_m);
    vec3f x = make_vec3(x2 - x1, 0, 0);
    vec3f y = make_vec3(0, y2 - y1, 0);
    vec3f z = make_vec3(0, 0, 1);
    ret.mat_m[INDEX4(0, 0)] = x[0];
    ret.mat_m[INDEX4(1, 0)] = x[1];
    ret.mat_m[INDEX4(2, 0)] = x[2];
    ret.mat_m[INDEX4(0, 1)] = y[0];
    ret.mat_m[INDEX4(1, 1)] = y[1];
    ret.mat_m[INDEX4(2, 1)] = y[2];
    ret.mat_m[INDEX4(0, 2)] = z[0];
    ret.mat_m[INDEX4(1, 2)] = z[1];
    ret.mat_m[INDEX4(2, 2)] = z[2];
    set_vec3f(x1, y1, 0, x);
    itrans_mat(x, ret.mat_m);
    free(x);
    free(y);
    free(z);
    ret.mode = GL_QUADS;
    ret.vbo = rect_vbo;
    ret.ibo = NULL;
    ret.count = 4;
    ret.material = _material;
    ret.free_mat_m = 1;
    ret.free_ibo = 0;
    return ret;
}
