#include <graphics/objects/sphere.h>

#include <math.h>
#include <assert.h>

#include <graphics/gl.h>
#include <math/constants.h>
#include <util/log.h>


unsigned int sphere_vbo;
vertex3d sphere_vbo_data[HEIGHT1][WIDTH];
short int sphere_ibo_data[HEIGHT][WIDTH][4];
#define IDX(I, J, K) (WIDTH * 4 * (I) + 4 * (J) + (K))

void init_sphere_object(void) {
    for (int i = 0; i < HEIGHT1; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            float alpha = j * 2 * M_PI / WIDTH;
            float beta = i * M_PI / HEIGHT;
            vertex3d cur;
            cur.normal[0] = cur.coord[0] = sinf(beta) * cosf(alpha);
            cur.normal[1] = cur.coord[1] = cosf(beta);
            cur.normal[2] = cur.coord[2] = sinf(beta) * sinf(alpha);
            sphere_vbo_data[i][j] = cur;
        }
    }
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            sphere_ibo_data[i][j][0] = i * WIDTH + j;
            sphere_ibo_data[i][j][1] = (i + 1) * WIDTH + j;
            sphere_ibo_data[i][j][2] = (i + 1) * WIDTH + ((j + 1) % WIDTH);
            sphere_ibo_data[i][j][3] = i* WIDTH + ((j + 1) % WIDTH);
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
