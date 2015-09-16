#ifndef GRAPHICS_OBJECTS_SPHERE_H
#define GRAPHICS_OBJECTS_SPHERE_H

#include <graphics/vertex3d.h>
#include <graphics/draw_obj.h>

#define HEIGHT 32
#define HEIGHT1 (HEIGHT + 1)
#define WIDTH (2 * HEIGHT)

#ifdef __cplusplus
extern "C" {
#endif


extern unsigned int sphere_vbo;
extern vertex3d sphere_vbo_data[HEIGHT1][WIDTH];
#define SPHERE_IBO_DATA_SIZE (HEIGHT * WIDTH * 4)
extern short int sphere_ibo_data[HEIGHT][WIDTH][4];
void init_sphere_object(void);
void free_sphere_object(void);
draw_obj make_draw_sphere1f(float radius, material_t _material);
draw_obj make_draw_sphere3fv1f(vec3f pos, float radius, material_t _material);


#ifdef __cplusplus
}
#endif

#endif // GRAPHICS_OBJECTS_SPHERE_H
