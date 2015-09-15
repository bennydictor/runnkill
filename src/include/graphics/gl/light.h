#ifndef GRAPHICS_GL_LIGHT_H
#define GRAPHICS_GL_LIGHT_H

#include <graphics/draw_obj_t.h>

extern unsigned int prog_light;

extern unsigned int prog_light_attr_v_coord, prog_light_attr_v_normal;
extern unsigned int prog_light_unif_z_near, prog_light_unif_z_far;
extern unsigned int prog_light_unif_mat_m, prog_light_unif_mat_v, prog_light_unif_mat_p;

int init_gl_light(void);
void render_light(int n, draw_obj_t *objs);
void free_gl_light(void);


#endif // GRAPHICS_GL_LIGHT_H
