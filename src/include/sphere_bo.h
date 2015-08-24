#ifndef SPHERE_BO_H
#define SPHERE_BO_H


#include <vertex3d.h>

#ifdef __cplusplus
extern "C" {
#endif

extern vertex3d sphere_vbo_data[200];
extern short int sphere_ibo_data[200][4];
extern short int sphere_lines_ibo_data[200][4][2];
void init_sphere(void);


#ifdef __cplusplus
}
#endif

#endif // SPHERE_BO_H
