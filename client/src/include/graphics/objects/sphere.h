#ifndef GRAPHICS_OBJECTS_SPHERE_H
#define GRAPHICS_OBJECTS_SPHERE_H

#include <graphics/vertex3d.h>
#include <graphics/draw_obj.h>

#define SPHERE_HEIGHT 32
#define SPHERE_HEIGHT1 (SPHERE_HEIGHT + 1)
#define SPHERE_WIDTH (2 * SPHERE_HEIGHT)

#ifdef __cplusplus
extern "C" {
#endif


extern unsigned int sphere_vbo;
extern vertex3d sphere_vbo_data[SPHERE_HEIGHT1][SPHERE_WIDTH];
#define SPHERE_IBO_DATA_SIZE (SPHERE_HEIGHT * SPHERE_WIDTH * 4)
extern unsigned int sphere_ibo_data[SPHERE_HEIGHT][SPHERE_WIDTH][4];
void init_sphere_object(void);
void free_sphere_object(void);
draw_obj make_draw_sphere1f(float radius, material_t _material);
draw_obj make_draw_sphere3fv1f(vec3f pos, float radius, material_t _material);


#ifdef __cplusplus
}
#endif

#endif // GRAPHICS_OBJECTS_SPHERE_H
