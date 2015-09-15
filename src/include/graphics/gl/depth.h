#ifndef GRAPHICS_GL_DEPTH_H
#define GRAPHICS_GL_DEPTH_H

#include <graphics/gl.h>


extern unsigned int prog_depth;
extern unsigned int fbo_depth[LIGHT_COUNT], fbo_depth_texture[LIGHT_COUNT];

extern unsigned int prog_depth_attr_v_coord;
extern unsigned int prog_depth_unif_mat_m, prog_depth_unif_mat_v, prog_depth_unif_mat_p;

int init_gl_depth(void);
void render_depth(int n, draw_obj_t *objs);
void free_gl_depth(void);


#endif // GRAPHICS_GL_DEPTH_H
