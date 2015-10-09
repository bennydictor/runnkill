#include <graphics/call_gl_on_display.h>
#include <graphics/draw_obj.h>
#include <graphics/gl.h>
#include <game/world.h>
#include <vector>

using namespace std;


void call_gl_on_display(float dt, char *but) {
    vector<draw_obj> res;
    cout << "ok draw" << endl;
    world_update(dt);
    cout << "ok draw" << endl;
    world_draw_objs(res);
    cout << "ok draw objs" << endl;
    man_update(0, but, vec3<float>(gl_rot[1], 0, gl_rot[0]));
    gl_on_display(res.size(), res.data());
    cout << "ok" << endl;
}
