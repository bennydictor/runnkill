#ifndef GRAPHICS_OBJECTS_CIRCLE_H
#define GRAPHICS_OBJECTS_CIRCLE_H

#include <graphics/vertex3d.h>
#include <graphics/draw_obj.h>

#define CIRCLE_SIZE 32

#ifdef __cplusplus
extern "C" {
#endif


extern unsigned int circle_vbo;
extern vertex3d circle_vbo_data[CIRCLE_SIZE];
void init_circle_object(void);
void free_circle_object(void);
draw_obj make_draw_circle_ny(vec3f pos, float radius, material_t _material);
draw_obj make_draw_circle(int x, int y, int radius, material_t _material);

#ifdef __cplusplus
}
#endif

#endif // GRAPHICS_OBJECTS_CIRCLE_H
