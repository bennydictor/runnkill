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

#include <net/net.h>

#ifdef _WIN32
#define printl3(X, Y, Z) printf(Y "\n", Z)
#define printl(X, Y) printf(Y "\n")
#else
#define printl3 printl
#endif

int main(int argc, char **argv) {
    lopen("/dev/stdout");
    min_log_level = LOG_D;
    if (argc < 2) {
        printl3(LOG_I, "Usage: %s hostname [port]", argv[0]);
        return EXIT_FAILURE;
    } else {
        if (init_net(argv[1], argc >= 3 ? atoi(argv[2]) : 0)) {
            printl(LOG_E, "Fatal error while initializing network.");
            return EXIT_FAILURE;
        }
    }

    ft_font_size = 16;

    gl_pos = make_vec3(-5, 3, 5);
    gl_rot = make_vec3(M_PI / 9, M_PI / 4, 0);
    gl_fov = 1;
    gl_z_near = .1;
    gl_z_far = 1000;

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
        if (controller()) {
            break;
        }
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
