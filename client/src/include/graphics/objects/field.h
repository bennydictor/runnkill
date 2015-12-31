#ifndef GRAPHICS_OBJECTS_FIELD_H
#define GRAPHICS_OBJECTS_FIELD_H

#include <graphics/vertex3d.h>
#include <graphics/draw_obj.h>

#ifdef __cplusplus
extern "C" {
#endif


extern unsigned int field_vbo;
extern vertex3d *field_vbo_data;
void init_field_object(int w, int h, int **bounds);
void free_field_object(void);
draw_obj make_draw_field(material_t _material);
draw_obj make_draw_subfield(int x1, int y1, int x2, int y2, material_t _material);


#ifdef __cplusplus
}
#endif

#endif // GRAPHICS_OBJECTS_FIELD_H
