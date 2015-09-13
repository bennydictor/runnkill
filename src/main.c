#include <stdlib.h>
#include <math.h>

#include <graphics/shader.h>
#include <graphics/glfw.h>
#include <graphics/gl.h>
#include <graphics/objects/sphere.h>
#include <graphics/objects/box.h>
#include <util/log.h>
#include <math/vecmath.h>
#include <math/constants.h>

#include <graphics/objects/sphere.h>
#include <graphics/objects/pp.h>

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

draw_obj_t objects[2];

void update(void) {
    float dt = delta();
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        pos[0] += sin(rot[1]) * dt;
        pos[2] -= cos(rot[1]) * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        pos[0] -= sin(rot[1]) * dt;
        pos[2] += cos(rot[1]) * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        pos[0] -= cos(rot[1]) * dt;
        pos[2] -= sin(rot[1]) * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        pos[0] += cos(rot[1]) * dt;
        pos[2] += sin(rot[1]) * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        pos[1] += dt;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        pos[1] -= dt;
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
    pos[0] *= -1;
    pos[1] *= -1;
    pos[2] *= -1;
    itrans_mat(pos, mat_v);
    pos[0] *= -1;
    pos[1] *= -1;
    pos[2] *= -1;
    irot_y_mat(rot[1], mat_v);
    irot_x_mat(rot[0], mat_v);

    id_mat4(light_mat_v);
    light_pos[0] *= -1;
    light_pos[1] *= -1;
    light_pos[2] *= -1;
    itrans_mat(light_pos, light_mat_v);
    light_pos[0] *= -1;
    light_pos[1] *= -1;
    light_pos[2] *= -1;
    irot_y_mat(light_rot[1], light_mat_v);
    irot_x_mat(light_rot[0], light_mat_v);

    gl_on_display(2, objects);
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
    
    objects[0].mode = GL_QUADS;
    objects[0].vbo = sphere_vbo;
    objects[0].ibo = sphere_ibo_data;
    objects[0].count = SPHERE_IBO_DATA_SIZE;
    objects[0].mat_m = make_mat4();
    vec3f v = make_vec3(0, 2, 0);
    trans_mat(v, objects[0].mat_m);
    objects[1].mode = GL_QUADS;
    objects[1].vbo = box_vbo;
    objects[1].ibo = NULL;
    objects[1].count = 4 * 6;
    objects[1].mat_m = make_mat4();
    free(v);
    v = make_vec3(10, 1, 10);
    scale_mat(v, objects[1].mat_m);
    free(v);
    v = make_vec3(-5, 0, -5);
    itrans_mat(v, objects[1].mat_m);
    free(v);

    while (!glfwWindowShouldClose(window)) {
        update();
    }

    free(objects[0].mat_m);
    free(objects[1].mat_m);

    free_gl();
    free_glfw();
    return EXIT_SUCCESS;
}
