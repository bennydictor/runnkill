#ifndef GRAPHICS_GL_H
#define GRAPHICS_GL_H

#include <math/vecmath.h>
#include <graphics/draw_obj_t.h>

#include <GL/glew.h>
#include <GL/gl.h>


extern unsigned int fbo_pp, rbo_pp, fbo_pp_texture;
extern mat4f light_mat_v, light_mat_p;
extern mat4f mat_v, mat_p;
extern vec3f light_pos, light_rot;
extern vec3f pos, rot;

int init_gl(void);
void gl_on_display(int n, draw_obj_t *count);
void free_gl(void);


#endif // GRAPHICS_GL_H
