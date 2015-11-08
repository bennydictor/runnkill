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

#include <game/init_world.h>


int main() {
    lopen("/dev/stderr");
    min_log_level = LOG_D;

    if (init_world()) {
        printl(LOG_E, "Fatal error while initializing world.");
        return EXIT_FAILURE;
    }
    for (;;) {
        controller();
    }

    free_world();
    lclose();
    return EXIT_SUCCESS;
}
