#include <controller.h>
#include <game/world.h>
#include <util/timing.h>
#include <net/client.h>
#include <vector>

using namespace std;

void controller(void) {
    float dt = delta();
    world_update(dt);
    world_callback();

    net_update();
    for (int i = 0; i < client_count; ++i) {
        if (clients[i].alive) {
            man_update(i, clients[i].evs, clients[i].orientation);
        }
    }
}
