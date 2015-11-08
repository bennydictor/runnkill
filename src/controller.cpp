#include <controller.h>
#include <graphics/gl.h>
#include <game/world.h>
#include <util/timing.h>
#include <vector>

using namespace std;

void controller(void) {
    float dt = delta();
    char evs[WORLD_EVENT_COUNT];
    world_update(dt, evs, gl_rot);
}
