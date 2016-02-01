#ifndef GRAPHICS_OBJECTS_RECT_H
#define GRAPHICS_OBJECTS_RECT_H

#include <graphics/vertex3d.h>
#include <graphics/draw_obj.h>
#include <math/vecmath.h>

#ifdef __cplusplus
extern "C" {
#endif


extern unsigned int rect_vbo;
extern vertex3d rect_vbo_data[4];
void init_rect_object(void);
void free_rect_object(void);
draw_obj make_draw_rect(int x1, int y1, int x2, int y2, material_t _material);


#ifdef __cplusplus
}
#endif

#endif // GRAPHICS_OBJECTS_RECT_H
