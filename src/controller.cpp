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
    float *hp, *mp;
    hp = new float[1];
    mp = new float[1];
    world_update(dt, evs, gl_rot, hp, mp);
    vector<draw_obj> res;

    world_callback(res, gl_pos);
    gl_update(res.size(), res.data(), hp[0], mp[0]);
    for (draw_obj obj : res) {
        free_draw_obj(obj);
    }
}
