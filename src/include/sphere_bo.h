#ifndef SPHERE_BO_H
#define SPHERE_BO_H


#include <vertex3d.h>

#ifdef __cplusplus
extern "C" {
#endif

<<<<<<< HEAD
extern vertex3d sphere_vbo_data[200];
extern short int sphere_ibo_data[200][4];
extern short int sphere_lines_ibo_data[200][4][2];
=======

#define HEIGHT 32
#define HEIGHT1 (HEIGHT + 1)
#define WIDTH (2 * HEIGHT)

extern vertex3d sphere_vbo_data[HEIGHT1][WIDTH];
extern short int sphere_ibo_data[HEIGHT][WIDTH][4];
>>>>>>> benny
void init_sphere(void);


#ifdef __cplusplus
}
#endif

#endif // SPHERE_BO_H
