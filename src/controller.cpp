#include <controller.h>
#include <graphics/gl.h>
#include <game/world.h>
#include <util/timing.h>
#include <vector>

using namespace std;

void controller(void) {
    float dt = delta();
    char evs[WORLD_EVENT_COUNT];
    gl_callback(evs);
    world_update(dt, evs, gl_rot);
    vector<draw_obj> res;
    world_callback(res, gl_pos);
    gl_update(res.size(), res.data());
    for (draw_obj obj : res) {
        free_draw_obj(obj);
    }
}
