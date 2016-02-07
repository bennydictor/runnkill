#include <graphics/objects/circle.h>

#include <math.h>

#include <graphics/gl.h>
#include <math/constants.h>
#include <util/log.h>


unsigned int circle_vbo;
vertex3d circle_vbo_data[CIRCLE_SIZE];

void init_circle_object(void) {
    for (int i = 0; i < CIRCLE_SIZE; ++i) {
        float alpha = i * 2 * M_PI / CIRCLE_SIZE;
        vertex3d cur;
        cur.coord[0] = cosf(alpha);
        cur.coord[1] = sinf(alpha);
        cur.normal[0] = cur.normal[1] = 0;
        cur.normal[2] = 1;
        cur.texcoord[0] = (cur.coord[0] + 1) / 2;
        cur.texcoord[1] = (cur.coord[1] + 1) / 2;
        circle_vbo_data[i] = cur;
    }
    glGenBuffers(1, &circle_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, circle_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle_vbo_data), circle_vbo_data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void free_circle_object(void) {
    glDeleteBuffers(1, &circle_vbo);
}

draw_obj make_draw_circle(int x, int y, int radius, material_t _material) {
    draw_obj ret;
    ret.mat_m = make_mat4();
    vec3f v = make_vec3(radius, radius, 1);
    scale_mat(v, ret.mat_m);
    set_vec3f(x, y, 0, v);
    itrans_mat(v, ret.mat_m);
    free(v);
    ret.mode = GL_POLYGON;
    ret.vbo = circle_vbo;
    ret.ibo = NULL;
    ret.count = CIRCLE_SIZE;
    ret.material = _material;
    ret.free_mat_m = 1;
    ret.free_ibo = 0;
    return ret;
}
