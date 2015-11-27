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

#include <graphics/objects/sphere_sector.h>
#include <graphics/objects/pp.h>
#include <controller.h>

#include <net.h>


int main(int argc, char **argv) {
    lopen("/dev/stderr");
    min_log_level = LOG_D;
    if (argc < 2) {
        printl(LOG_I, "Usage: %s hostname [port]", argv[0]);
        return EXIT_FAILURE;
    } else {
        if (init_net(argv[1], argc >= 3 ? atoi(argv[2]) : 0)) {
            printl(LOG_E, "Fatal error while initializing network.");
            return EXIT_FAILURE;
        }
    }

    ft_font_size = 24;

    gl_pos = make_vec3(-5, 3, 5);
    gl_rot = make_vec3(M_PI / 9, M_PI / 4, 0);
    gl_fov = 1;
    gl_z_near = .1;
    gl_z_far = 1000;

    float ones[] = {1, 1, 1};

    gl_light_enable[0] = 1;
    gl_light[0].pos = make_vec3(100, 100, 100);
    gl_light[0].rot = make_vec3(3 * M_PI / 2, 0, 0);
    gl_light[0].fov = 1;
    gl_light[0].z_near = .1;
    gl_light[0].z_far = 1000;
    gl_light[0].ambient = make_vec3(.3, .3, .3);
    gl_light[0].diffuse = make_vec3(.4, .4, .4);
    gl_light[0].specular = ones;

    if (init_glfw()) {
        printl(LOG_E, "Fatal error while initializing glfw.");
        return EXIT_FAILURE;
    }
    if (init_gl()) {
        printl(LOG_E, "Fatal error while initializing gl.");
        free_glfw();
        return EXIT_FAILURE;
    }
    while (!glfwWindowShouldClose(window)) {
        controller();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            break;
        }
    }

    free_gl();
    free_glfw();
    free_net();
    lclose();
    return EXIT_SUCCESS;
}
