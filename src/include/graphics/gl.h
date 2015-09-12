#ifndef GRAPHICS_GL_H
#define GRAPHICS_GL_H

#include <math/vecmath.h>

#include <GL/glew.h>
#include <GL/gl.h>


extern unsigned int fbo, rbo, fbo_texture;
extern mat4f mat_m, mat_v, mat_p;

int init_gl(void);
void gl_on_display(void);
void free_gl(void);


#endif // GRAPHICS_GL_H
