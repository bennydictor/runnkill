#include <graphics/objects/box.h>
#include <graphics/gl.h>


unsigned int box_vbo;
vertex3d box_vbo_data[4 * 6] = {
    {{0, 0, 0}, {-1, 0, 0}},
    {{0, 1, 0}, {-1, 0, 0}},
    {{0, 1, 1}, {-1, 0, 0}},
    {{0, 0, 1}, {-1, 0, 0}},

    {{1, 0, 0}, {+1, 0, 0}},
    {{1, 1, 0}, {+1, 0, 0}},
    {{1, 1, 1}, {+1, 0, 0}},
    {{1, 0, 1}, {+1, 0, 0}},

    {{0, 0, 0}, {0, -1, 0}},
    {{1, 0, 0}, {0, -1, 0}},
    {{1, 0, 1}, {0, -1, 0}},
    {{0, 0, 1}, {0, -1, 0}},

    {{0, 1, 0}, {0, +1, 0}},
    {{1, 1, 0}, {0, +1, 0}},
    {{1, 1, 1}, {0, +1, 0}},
    {{0, 1, 1}, {0, +1, 0}},

    {{0, 0, 0}, {0, 0, -1}},
    {{1, 0, 0}, {0, 0, -1}},
    {{1, 1, 0}, {0, 0, -1}},
    {{0, 1, 0}, {0, 0, -1}},

    {{0, 0, 1}, {0, 0, +1}},
    {{1, 0, 1}, {0, 0, +1}},
    {{1, 1, 1}, {0, 0, +1}},
    {{0, 1, 1}, {0, 0, +1}},
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
