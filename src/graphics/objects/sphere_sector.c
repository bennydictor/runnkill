#include <graphics/objects/sphere_sector.h>

#include <math.h>

#include <graphics/gl.h>
#include <math/constants.h>


unsigned int sphere_sector_vbo;
vertex3d sphere_sector_vbo_data[SPHERE_SECTOR_HEIGHT1 * SPHERE_SECTOR_WIDTH1 + SPHERE_SECTOR_WIDTH1 + 2 * SPHERE_SECTOR_HEIGHT1 + 3];
short int sphere_sector_ibo_data[SPHERE_SECTOR_IBO_DATA_SIZE];

void init_sphere_sector_object(void) {
    int idx = 0;
    for (int i = 0; i < SPHERE_SECTOR_HEIGHT1; ++i) {
        for (int j = 0; j < SPHERE_SECTOR_WIDTH1; ++j, ++idx) {
            float alpha = j * M_PI / 2 / SPHERE_SECTOR_WIDTH;
            float beta = i * M_PI / 2 / SPHERE_SECTOR_HEIGHT;
            vertex3d cur;
            cur.normal[0] = cur.coord[0] = sinf(beta) * cosf(alpha);
            cur.normal[1] = cur.coord[1] = cosf(beta);
            cur.normal[2] = cur.coord[2] = sinf(beta) * sinf(alpha);
            sphere_sector_vbo_data[idx] = cur;
        }
    }
    int offset1 = idx;
    for (int i = 0; i < SPHERE_SECTOR_WIDTH1; ++i, ++idx) {
        float alpha = i * M_PI / 2 / SPHERE_SECTOR_WIDTH;
        vertex3d cur;
        cur.coord[0] = cosf(alpha);
        cur.coord[1] = 0;
        cur.coord[2] = sinf(alpha);
        cur.normal[0] = 0;
        cur.normal[1] = -1;
        cur.normal[2] = 0;
        sphere_sector_vbo_data[idx] = cur;
    }
    {
        vertex3d cur;
        cur.coord[0] = 0;
        cur.coord[1] = 0;
        cur.coord[2] = 0;
        cur.normal[0] = 0;
        cur.normal[1] = -1;
        cur.normal[2] = 0;
        sphere_sector_vbo_data[idx++] = cur;
    }
    int offset2 = idx;
    for (int i = 0; i < SPHERE_SECTOR_HEIGHT1; ++i, ++idx) {
        float beta = i * M_PI / 2 / SPHERE_SECTOR_HEIGHT;
        vertex3d cur;
        cur.coord[0] = sinf(beta);
        cur.coord[1] = cosf(beta);
        cur.coord[2] = 0;
        cur.normal[0] = 0;
        cur.normal[1] = 0;
        cur.normal[2] = -1;
        sphere_sector_vbo_data[idx] = cur;
    }
    {
        vertex3d cur;
        cur.coord[0] = 0;
        cur.coord[1] = 0;
        cur.coord[2] = 0;
        cur.normal[0] = 0;
        cur.normal[1] = 0;
        cur.normal[2] = -1;
        sphere_sector_vbo_data[idx++] = cur;
    }
    int offset3 = idx;
    for (int i = 0; i < SPHERE_SECTOR_HEIGHT1; ++i, ++idx) {
        float beta = i * M_PI / 2 / SPHERE_SECTOR_HEIGHT;
        vertex3d cur;
        cur.coord[0] = 0;
        cur.coord[1] = cosf(beta);
        cur.coord[2] = sinf(beta);
        cur.normal[0] = -1;
        cur.normal[1] = 0;
        cur.normal[2] = 0;
        sphere_sector_vbo_data[idx] = cur;
    }
    {
        vertex3d cur;
        cur.coord[0] = 0;
        cur.coord[1] = 0;
        cur.coord[2] = 0;
        cur.normal[0] = -1;
        cur.normal[1] = 0;
        cur.normal[2] = 0;
        sphere_sector_vbo_data[idx++] = cur;
    }
    idx = 0;
    for (int i = 0; i < SPHERE_SECTOR_HEIGHT; i++) {
        for (int j = 0; j < SPHERE_SECTOR_WIDTH; j++) {
            sphere_sector_ibo_data[idx++] = i * SPHERE_SECTOR_WIDTH1 + j;
            sphere_sector_ibo_data[idx++] = (i + 1) * SPHERE_SECTOR_WIDTH1 + j;
            sphere_sector_ibo_data[idx++] = (i + 1) * SPHERE_SECTOR_WIDTH1 + j + 1;
            sphere_sector_ibo_data[idx++] = i * SPHERE_SECTOR_WIDTH1 + j;
            sphere_sector_ibo_data[idx++] = (i + 1) * SPHERE_SECTOR_WIDTH1 + j + 1;
            sphere_sector_ibo_data[idx++] = i * SPHERE_SECTOR_WIDTH1 + j + 1;
        }
    }
    for (int i = 0; i < SPHERE_SECTOR_WIDTH; ++i) {
        sphere_sector_ibo_data[idx++] = offset1 + i;
        sphere_sector_ibo_data[idx++] = offset1 + i + 1;
        sphere_sector_ibo_data[idx++] = offset1 + SPHERE_SECTOR_WIDTH1;
    }
    for (int i = 0; i < SPHERE_SECTOR_HEIGHT; ++i) {
        sphere_sector_ibo_data[idx++] = offset2 + i;
        sphere_sector_ibo_data[idx++] = offset2 + i + 1;
        sphere_sector_ibo_data[idx++] = offset2 + SPHERE_SECTOR_HEIGHT1;
    }
    for (int i = 0; i < SPHERE_SECTOR_HEIGHT; ++i) {
        sphere_sector_ibo_data[idx++] = offset3 + i;
        sphere_sector_ibo_data[idx++] = offset3 + i + 1;
        sphere_sector_ibo_data[idx++] = offset3 + SPHERE_SECTOR_HEIGHT1;
    }
    glGenBuffers(1, &sphere_sector_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sphere_sector_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_sector_vbo_data), sphere_sector_vbo_data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void free_sphere_sector_object(void) {
    glDeleteBuffers(1, &sphere_sector_vbo);
}

#define SIGN_BIT(N, B) (((N) & (1 << (B))) ? (1) : (-1))

draw_obj make_draw_sphere_sector1f(float radius, int s, material_t _material) {
    draw_obj ret;
    ret.mat_m = make_mat4();
    vec3f v = make_vec3(radius * SIGN_BIT(s, 2), radius * SIGN_BIT(s, 1), radius * SIGN_BIT(s, 0));
    scale_mat(v, ret.mat_m);
    free(v);
    ret.mode = GL_TRIANGLES;
    ret.vbo = sphere_sector_vbo;
    ret.ibo = sphere_sector_ibo_data;
    ret.count = SPHERE_SECTOR_IBO_DATA_SIZE;
    ret.material = _material;
    return ret;
}

draw_obj make_draw_sphere_sector3fv1f(vec3f pos, float radius, int s, material_t _material) {
    draw_obj ret;
    ret.mat_m = make_mat4();
    vec3f v = make_vec3(radius * SIGN_BIT(s, 2), radius * SIGN_BIT(s, 1), radius * SIGN_BIT(s, 0));
    scale_mat(v, ret.mat_m);
    free(v);
    itrans_mat(pos, ret.mat_m);
    ret.mode = GL_TRIANGLES;
    ret.vbo = sphere_sector_vbo;
    ret.ibo = sphere_sector_ibo_data;
    ret.count = SPHERE_SECTOR_IBO_DATA_SIZE;
    ret.material = _material;
    return ret;
}

