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
draw_obj make_draw_rect(float x1, float y1, float x2, float y2, material_t _material);
draw_obj make_draw_rect4f1f(float x1, float y1, float x2, float y2, float z1, float rotate, material_t _material);


#ifdef __cplusplus
}
#endif

#endif // GRAPHICS_OBJECTS_RECT_H
