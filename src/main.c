#include <stdlib.h>
#include <math.h>

#include <util/log.h>
#include <util/timing.h>
#include <math/vecmath.h>
#include <math/constants.h>
#include <controller.h>

#include <net/socket.h>
#include <game/init_world.h>
#include <signal.h>
#include <net/client.h>


volatile char interrupt;
void signal_handler(int signum) {
    if (signum == SIGINT) {
        printl(LOG_I, "Catched SIGINT");
    }
    interrupt = 1;
}

int main() {
    lopen("/dev/stderr");
    min_log_level = LOG_D;

    if (init_world()) {
        printl(LOG_E, "Fatal error while initializing world");
        return EXIT_FAILURE;
    }
    if (init_net()) {
        printl(LOG_E, "Fatal error while initializing network");
        free_world();
        return EXIT_FAILURE;
    }
    signal(SIGINT, signal_handler);
    interrupt = 0;
    while (!interrupt) {
        controller();
    }

    free_net();
    free_world();
    lclose();
    return EXIT_SUCCESS;
}
