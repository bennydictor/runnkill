#ifndef GRAPHICS_GL_PP_H
#define GRAPHICS_GL_PP_H


extern unsigned int prog_pp;
extern unsigned int fbo_pp, rbo_pp, fbo_pp_texture;

extern unsigned int prog_pp_attr_v_coord;
extern unsigned int prog_pp_unif_texture;

int init_gl_pp(void);
void render_pp(void);
void free_gl_pp(void);


#endif // GRAPHICS_GL_PP_H
