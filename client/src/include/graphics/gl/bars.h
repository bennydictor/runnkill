#ifndef GRAPHICS_GL_BARS_H
#define GRAPHICS_GL_BARS_H

#include <graphics/draw_obj.h>

unsigned int prog_bars;

unsigned int prog_bars_attr_v_coord;
unsigned int prog_bars_unif_mat_m, prog_bars_unif_mat_p;

int init_gl_bars(void);
void render_bars(void);
void free_gl_bars(void);


#endif // GRAPHICS_GL_BARS_H
