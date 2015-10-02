#include <graphics/call_gl_on_display.h>
#include <graphics/draw_obj.h>
#include <graphics/gl.h>
#include <game/world.h>
#include <vector>

using namespace std;


void call_gl_on_display(float dt) {
    vector<draw_obj> res;
    world_update(dt);
    world_draw_objs(res);
    gl_on_display(res.size(), res.data());
}
