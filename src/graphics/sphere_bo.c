#include <graphics/sphere_bo.h>

#include <math.h>
#include <assert.h>

#include <math/pi.h>
#include <util/log.h>


vertex3d sphere_vbo_data[HEIGHT1][WIDTH];
short int sphere_ibo_data[HEIGHT][WIDTH][4];

void init_sphere(void) {
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
}
