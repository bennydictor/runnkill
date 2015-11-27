#include <controller.h>
#include <graphics/gl.h>
#include <game/world.h>
#include <util/timing.h>
#include <vector>

using namespace std;

void controller(void) {
    char evs[WORLD_EVENT_COUNT];
    vector<draw_obj> res;
    gl_callback(evs);
    gl_update(res.size(), res.data());
    for (draw_obj obj : res) {
        free_draw_obj(obj);
    }
}
