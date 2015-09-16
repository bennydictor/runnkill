#include <stdlib.h>
#include <math.h>

#include <graphics/shader.h>
#include <graphics/glfw.h>
#include <graphics/gl.h>
#include <graphics/ft.h>
#include <graphics/objects/sphere.h>
#include <graphics/objects/box.h>
#include <util/log.h>
#include <util/timing.h>
#include <math/vecmath.h>
#include <math/constants.h>

#include <graphics/objects/sphere.h>
#include <graphics/objects/pp.h>


draw_obj objects[2];

void update(void) {
    float dt = delta();
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        gl_pos[0] += sin(gl_rot[1]) * dt;
        gl_pos[2] -= cos(gl_rot[1]) * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        gl_pos[0] -= sin(gl_rot[1]) * dt;
        gl_pos[2] += cos(gl_rot[1]) * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        gl_pos[0] -= cos(gl_rot[1]) * dt;
        gl_pos[2] -= sin(gl_rot[1]) * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        gl_pos[0] += cos(gl_rot[1]) * dt;
        gl_pos[2] += sin(gl_rot[1]) * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        gl_pos[1] += dt;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        gl_pos[1] -= dt;
    }
    gl_rot[0] += cursor_dy / 100.0;
    gl_rot[1] += cursor_dx / 100.0;
    cursor_dx = cursor_dy = 0;
    if (gl_rot[0] < -M_PI / 2) {
        gl_rot[0] = -M_PI / 2;
    }
    if (gl_rot[0] > +M_PI / 2) {
        gl_rot[0] = +M_PI / 2;
    }
    if (gl_rot[1] < -2 * M_PI) {
        gl_rot[1] += 2 * M_PI;
    }
    if (gl_rot[1] > +2 * M_PI) {
        gl_rot[1] -= 2 * M_PI;
    }

    gl_on_display(2, objects);
}

int main() {
    lopen("/dev/stderr");
    min_log_level = LOG_D;
    ft_font_size = 24;

    gl_pos = make_vec3(-5, 3, 5);
    gl_rot = make_vec3(M_PI / 9, M_PI / 4, 0);
    gl_fov = 1;
    gl_z_near = .1;
    gl_z_far = 100;

    vec3f ones = make_vec3(1, 1, 1);

    gl_light_enable[0] = 1;
    gl_light[0].pos = make_vec3(-3, 5, 10);
    gl_light[0].rot = make_vec3(M_PI / 8, 0, 0);
    gl_light[0].fov = 1;
    gl_light[0].z_near = .1;
    gl_light[0].z_far = 100;
    gl_light[0].ambient = make_vec3(.1, .1, .1);
    gl_light[0].diffuse = make_vec3(.4, .4, .4);
    gl_light[0].specular = ones;

    gl_light_enable[1] = 1;
    gl_light[1].pos = make_vec3(+3, 5, 10);
    gl_light[1].rot = make_vec3(M_PI / 8, 0, 0);
    gl_light[1].fov = 1;
    gl_light[1].z_near = .1;
    gl_light[1].z_far = 100;
    gl_light[1].ambient = make_vec3(.1, .1, .1);
    gl_light[1].diffuse = make_vec3(.4, .4, .4);
    gl_light[1].specular = ones;

    if (init_glfw()) {
        printl(LOG_E, "Fatal error while initializing glfw.");
        return EXIT_FAILURE;
    }
    if (init_gl()) {
        printl(LOG_E, "Fatal error while initializing gl.");
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
    free(v);
    objects[0].material = make_material(ones, ones, ones, 128);
    objects[1].mode = GL_QUADS;
    objects[1].vbo = box_vbo;
    objects[1].ibo = NULL;
    objects[1].count = 4 * 6;
    objects[1].mat_m = make_mat4();
    objects[1].material = make_material(ones, ones, make_vec3(0, 0, 0), 1);
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
