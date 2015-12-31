#include <graphics/draw_obj.h>

void free_draw_obj(draw_obj obj) {
    if (obj.free_mat_m) {
        free(obj.mat_m);
    }
    if (obj.free_ibo) {
        free(obj.ibo);
    }
}
