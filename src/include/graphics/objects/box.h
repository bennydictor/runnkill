#ifndef GRAPHICS_OBJECTS_BOX_H
#define GRAPHICS_OBJECTS_BOX_H

#include <graphics/vertex3d.h>
#include <graphics/draw_obj.h>
#include <math/vecmath.h>

#ifdef __cplusplus
extern "C" {
#endif


extern unsigned int box_vbo;
extern vertex3d box_vbo_data[4 * 6];
void init_box_object(void);
void free_box_object(void);
draw_obj make_draw_box(vec3f *bounds, material_t _material);


#ifdef __cplusplus
}
#endif

#endif // GRAPHICS_OBJECTS_BOX_H
