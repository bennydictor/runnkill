#ifndef SPHERE_BO_H
#define SPHERE_BO_H

#include <graphics/vertex3d.h>

extern unsigned int box_vbo;
extern vertex3d box_vbo_data[4 * 6];
void init_box_object(void);
void free_box_object(void);


#endif // SPHERE_BO_H
