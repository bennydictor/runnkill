#include <graphics/objects/sphere.h>

#include <math.h>
#include <assert.h>

#include <graphics/gl.h>
#include <math/constants.h>
#include <util/log.h>


unsigned int sphere_vbo;
vertex3d sphere_vbo_data[SPHERE_HEIGHT1][SPHERE_WIDTH];
unsigned int sphere_ibo_data[SPHERE_HEIGHT][SPHERE_WIDTH][4];
#define IDX(I, J, K) (SPHERE_WIDTH * 4 * (I) + 4 * (J) + (K))

void init_sphere_object(void) {
    for (int i = 0; i < SPHERE_HEIGHT1; ++i) {
        for (int j = 0; j < SPHERE_WIDTH; ++j) {
            float alpha = j * 2 * M_PI / SPHERE_WIDTH;
            float beta = i * M_PI / SPHERE_HEIGHT;
            vertex3d cur;
            cur.normal[0] = cur.coord[0] = sinf(beta) * cosf(alpha);
            cur.normal[1] = cur.coord[1] = cosf(beta);
            cur.normal[2] = cur.coord[2] = sinf(beta) * sinf(alpha);
            cur.texcoord[0] = ((float) j) / SPHERE_WIDTH;
            cur.texcoord[1] = ((float) i) / SPHERE_HEIGHT;
            sphere_vbo_data[i][j] = cur;
        }
    }
    for (int i = 0; i < SPHERE_HEIGHT; i++) {
        for (int j = 0; j < SPHERE_WIDTH; j++) {
            sphere_ibo_data[i][j][0] = i * SPHERE_WIDTH + j;
            sphere_ibo_data[i][j][1] = (i + 1) * SPHERE_WIDTH + j;
            sphere_ibo_data[i][j][2] = (i + 1) * SPHERE_WIDTH + ((j + 1) % SPHERE_WIDTH);
            sphere_ibo_data[i][j][3] = i* SPHERE_WIDTH + ((j + 1) % SPHERE_WIDTH);
        }
    }
    glGenBuffers(1, &sphere_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vbo_data), sphere_vbo_data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void free_sphere_object(void) {
    glDeleteBuffers(1, &sphere_vbo);
}

draw_obj make_draw_sphere1f(float radius, material_t _material) {
    draw_obj ret;
    ret.mat_m = make_mat4();
    vec3f v = make_vec3(radius, radius, radius);
    scale_mat(v, ret.mat_m);
    free(v);
    ret.mode = GL_QUADS;
    ret.vbo = sphere_vbo;
    ret.ibo = sphere_ibo_data;
    ret.count = SPHERE_IBO_DATA_SIZE;
    ret.material = _material;
    ret.free_mat_m = 1;
    ret.free_ibo = 0;
    return ret;
}

draw_obj make_draw_sphere3fv1f(vec3f pos, float radius, material_t _material) {
    draw_obj ret;
    ret.mat_m = make_mat4();
    vec3f v = make_vec3(radius, radius, radius);
    scale_mat(v, ret.mat_m);
    free(v);
    itrans_mat(pos, ret.mat_m);
    ret.mode = GL_QUADS;
    ret.vbo = sphere_vbo;
    ret.ibo = sphere_ibo_data;
    ret.count = SPHERE_IBO_DATA_SIZE;
    ret.material = _material;
    ret.free_mat_m = 1;
    ret.free_ibo = 0;
    return ret;
}

