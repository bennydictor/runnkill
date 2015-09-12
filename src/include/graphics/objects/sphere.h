#ifndef GRAPHICS_OBJECTS_SPHERE_H
#define GRAPHICS_OBJECTS_SPHERE_H


#include <graphics/vertex3d.h>

#define HEIGHT 32
#define HEIGHT1 (HEIGHT + 1)
#define WIDTH (2 * HEIGHT)

extern vertex3d sphere_vbo_data[HEIGHT1][WIDTH];
extern short int sphere_ibo_data[HEIGHT][WIDTH][4];
void init_sphere(void);


#endif // GRAPHICS_OBJECTS_SPHERE_H
