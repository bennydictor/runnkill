#include <graphics/objects/box.h>
#include <graphics/gl.h>
#include <string.h>


unsigned int box_vbo;
vertex3d box_vbo_data[4 * 6] = {
    {{0, 0, 0}, {0, 0}, {-1, 0, 0}},
    {{0, 1, 0}, {1, 0}, {-1, 0, 0}},
    {{0, 1, 1}, {1, 1}, {-1, 0, 0}},
    {{0, 0, 1}, {0, 1}, {-1, 0, 0}},

    {{1, 0, 0}, {0, 0}, {+1, 0, 0}},
    {{1, 1, 0}, {1, 0}, {+1, 0, 0}},
    {{1, 1, 1}, {1, 1}, {+1, 0, 0}},
    {{1, 0, 1}, {0, 1}, {+1, 0, 0}},

    {{0, 0, 0}, {0, 0}, {0, -1, 0}},
    {{1, 0, 0}, {1, 0}, {0, -1, 0}},
    {{1, 0, 1}, {1, 1}, {0, -1, 0}},
    {{0, 0, 1}, {0, 1}, {0, -1, 0}},

    {{0, 1, 0}, {0, 0}, {0, +1, 0}},
    {{1, 1, 0}, {1, 0}, {0, +1, 0}},
    {{1, 1, 1}, {1, 1}, {0, +1, 0}},
    {{0, 1, 1}, {0, 1}, {0, +1, 0}},

    {{0, 0, 0}, {0, 0}, {0, 0, -1}},
    {{1, 0, 0}, {1, 0}, {0, 0, -1}},
    {{1, 1, 0}, {1, 1}, {0, 0, -1}},
    {{0, 1, 0}, {0, 1}, {0, 0, -1}},

    {{0, 0, 1}, {0, 0}, {0, 0, +1}},
    {{1, 0, 1}, {1, 0}, {0, 0, +1}},
    {{1, 1, 1}, {1, 1}, {0, 0, +1}},
    {{0, 1, 1}, {0, 1}, {0, 0, +1}},
};

void init_box_object(void) {
    glGenBuffers(1, &box_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, box_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(box_vbo_data), box_vbo_data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void free_box_object(void) {
    glDeleteBuffers(1, &box_vbo);
}

draw_obj make_draw_box(vec3f *bounds, material_t _material) {
    draw_obj ret;
    ret.mat_m = make_mat4();
    id_mat4(ret.mat_m);
    vec3f x = sub_vec3fv(copy_vec3(bounds[1]), bounds[0]);
    vec3f y = sub_vec3fv(copy_vec3(bounds[2]), bounds[0]);
    vec3f z = sub_vec3fv(copy_vec3(bounds[3]), bounds[0]);
    memcpy(ret.mat_m +  0, x, 3 * sizeof(float));
    memcpy(ret.mat_m +  4, y, 3 * sizeof(float));
    memcpy(ret.mat_m +  8, z, 3 * sizeof(float));
    free(x);
    free(y);
    free(z);
    itrans_mat(bounds[0], ret.mat_m);
    ret.mode = GL_QUADS;
    ret.vbo = box_vbo;
    ret.ibo = NULL;
    ret.count = 4 * 6;
    ret.material = _material;
    return ret;
}
