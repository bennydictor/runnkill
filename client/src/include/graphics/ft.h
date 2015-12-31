#ifndef GRAPHICS_TEXT_H
#define GRAPHICS_TEXT_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <graphics/vertex3d.h>


typedef struct {
    int size_x;
    int size_y;
    int bearing_x;
    int bearing_y;
    int advance;
    float texcoord_x;
} ft_char;

extern unsigned int ft_font_size;
extern unsigned int prog_ft;
extern unsigned int prog_ft_attr_v_coord;
extern unsigned int prog_ft_unif_mat_p, prog_ft_attr_texture, prog_ft_attr_color;

int init_ft(void);
void render_text(int x, int y, const char *str);
void free_ft(void);

#endif // GRAPHICS_TEXT_H
