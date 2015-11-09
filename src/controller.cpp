#include <controller.h>
#include <graphics/gl.h>
#include <game/world.h>
#include <util/timing.h>
#include <vector>

using namespace std;

void controller(void) {
    float dt = delta();
    world_update(dt);
}
