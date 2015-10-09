#include <graphics/call_gl_on_display.h>
#include <graphics/draw_obj.h>
#include <graphics/gl.h>
#include <game/world.h>
#include <vector>

using namespace std;


void call_gl_on_display(float dt, char *but) {
    vector<draw_obj> res;
    world_update(dt);
    world_draw_objs(res);
    man_update(0, but, vec3<float>(-cosf(gl_rot[1]), 0, sinf(gl_rot[1])));
    world_get_coords(gl_pos);
    gl_on_display(res.size(), res.data());
}
