#ifndef GRAPHICS_OBJECTS_SPHERE_SECTOR_H
#define GRAPHICS_OBJECTS_SPHERE_SECTOR_H

#include <graphics/vertex3d.h>
#include <graphics/draw_obj.h>

#define SPHERE_SECTOR_HEIGHT 16
#define SPHERE_SECTOR_WIDTH1 (SPHERE_SECTOR_WIDTH + 1)
#define SPHERE_SECTOR_HEIGHT1 (SPHERE_SECTOR_HEIGHT + 1)
#define SPHERE_SECTOR_WIDTH (2 * SPHERE_SECTOR_HEIGHT)

#ifdef __cplusplus
extern "C" {
#endif


extern unsigned int sphere_sector_vbo;
extern vertex3d sphere_sector_vbo_data[SPHERE_SECTOR_HEIGHT1 * SPHERE_SECTOR_WIDTH1 + SPHERE_SECTOR_WIDTH1 + 2 * SPHERE_SECTOR_HEIGHT1 + 3];
#define SPHERE_SECTOR_IBO_DATA_SIZE (SPHERE_SECTOR_HEIGHT * SPHERE_SECTOR_WIDTH * 6 + SPHERE_SECTOR_WIDTH * 3 + 2 * SPHERE_SECTOR_HEIGHT * 3)
extern short int sphere_sector_ibo_data[SPHERE_SECTOR_IBO_DATA_SIZE];
void init_sphere_sector_object(void);
void free_sphere_sector_object(void);
draw_obj make_draw_sphere_sector1f(float radius, int s, material_t _material);
draw_obj make_draw_sphere_sector3fv1f(vec3f pos, float radius, int s, material_t _material);


#ifdef __cplusplus
}
#endif

#endif // GRAPHICS_OBJECTS_SPHERE_SECTOR_H
