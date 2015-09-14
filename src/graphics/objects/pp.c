#include <graphics/objects/pp.h>
#include <graphics/gl.h>


unsigned int pp_vbo;
vertex3d pp_vbo_data[4] = {
    {{-1, -1, 0}, {0, 0, 0}},
    {{+1, -1, 0}, {0, 0, 0}},
    {{+1, +1, 0}, {0, 0, 0}},
    {{-1, +1, 0}, {0, 0, 0}},
};

void init_pp_object(void) {
    glGenBuffers(1, &pp_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, pp_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pp_vbo_data), pp_vbo_data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void free_pp_object(void) {
    glDeleteBuffers(1, &pp_vbo);
}
