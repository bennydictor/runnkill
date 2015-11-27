#include <controller.h>
#include <graphics/gl.h>
#include <game/world.h>
#include <util/timing.h>
#include <net/client.h>
#include <vector>

using namespace std;

void controller(void) {
    float dt = delta();
    net_update();
    world_update(dt);
}
