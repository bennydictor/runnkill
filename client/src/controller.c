#include <controller.h>
#include <graphics/gl.h>
#include <util/timing.h>
#include <net/net.h>
#include <world.h>

#define MAX_DRAW_OBJ 256

int controller(void) {
    delta();
    char evs[WORLD_EVENT_COUNT];
    draw_obj draw_objs[MAX_DRAW_OBJ];
    int draw_obj_count;
    gl_callback(evs);
    if (net_update(evs, &draw_obj_count, draw_objs)) {
        return 1;
    }
    gl_update(draw_obj_count, draw_objs);
    for (int i = 0; i < draw_obj_count; ++i) {
        free_draw_obj(draw_objs[i]);
    }
    return 0;
}
