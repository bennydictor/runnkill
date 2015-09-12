#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include <graphics/shader.h>
#include <graphics/glfw.h>
#include <graphics/gl.h>
#include <util/log.h>
#include <math/vecmath.h>
#include <math/constants.h>

#include <graphics/objects/sphere.h>
#include <graphics/objects/pp.h>

vec3f pos, rot;

float prev_time;
unsigned int fps;
float last_point;

float delta() {
    ++fps;
    float cur_time = glfwGetTime();
    if (cur_time - last_point >= 1) {
        printl(LOG_D, "fps: %d\n", fps);
        fps = 0;
        last_point = cur_time;
    }
    float d = cur_time - prev_time;
    prev_time = cur_time;
    return d;
}

void update(void) {
    float dt = delta();
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        pos[0] -= sin(rot[1]) * dt;
        pos[2] += cos(rot[1]) * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        pos[0] += sin(rot[1]) * dt;
        pos[2] -= cos(rot[1]) * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        pos[0] += cos(rot[1]) * dt;
        pos[2] += sin(rot[1]) * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        pos[0] -= cos(rot[1]) * dt;
        pos[2] -= sin(rot[1]) * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        pos[1] -= dt;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        pos[1] += dt;
    }
    rot[0] += cursor_dy / 100.0;
    rot[1] += cursor_dx / 100.0;
    cursor_dx = cursor_dy = 0;
    if (rot[0] < -M_PI / 2) {
        rot[0] = -M_PI / 2;
    }
    if (rot[0] > +M_PI / 2) {
        rot[0] = +M_PI / 2;
    }
    if (rot[1] < -2 * M_PI) {
        rot[1] += 2 * M_PI;
    }
    if (rot[1] > +2 * M_PI) {
        rot[1] -= 2 * M_PI;
    }
    id_mat4(mat_v);
    itrans_mat(pos, mat_v);
    irot_y_mat(rot[1], mat_v);
    irot_x_mat(rot[0], mat_v);

    gl_on_display();
}

int main() {
    lopen("/dev/stderr");
    min_log_level = LOG_D;

    if (init_glfw() == -1) {
        return EXIT_FAILURE;
    }
    if (init_gl() == -1) {
        free_glfw();
        return EXIT_FAILURE;
    }
    while (!glfwWindowShouldClose(window)) {
        update();
    }

    free_gl();
    free_glfw();
    return EXIT_SUCCESS;
}
