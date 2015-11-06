#ifndef GRAPHICS_OBJECTS_PP_H
#define GRAPHICS_OBJECTS_PP_H

#include <graphics/vertex3d.h>


extern unsigned int pp_vbo;
extern vertex3d pp_vbo_data[4];
void init_pp_object(void);
void free_pp_object(void);


#endif // GRAPHICS_OBJECTS_PP_H
