#include <graphics/ft.h>
#include <util/log.h>
#include <graphics/gl.h>
#include <graphics/gl/pp.h>
#include <graphics/glfw.h>
#include <math/vecmath.h>


ft_char ft_chars[128];

FT_Library lib;
FT_Face face;
unsigned int ft_font_size;

mat4f ft_mat_p;

unsigned int prog_ft;

unsigned int prog_ft_attr_v_coord, prog_ft_attr_v_texcoord;
unsigned int prog_ft_unif_mat_p, prog_ft_unif_texture, prog_ft_unif_colors;

unsigned int ft_vbo, textures[128];
vertex3d ft_vbo_data[4];

int init_ft(void) {
    INIT_PROG(ft);

    INIT_ATTR(prog_ft, v_coord);
    INIT_ATTR(prog_ft, v_texcoord);
 
    INIT_UNIF(prog_ft, mat_p);
    INIT_UNIF(prog_ft, texture);
    INIT_UNIF(prog_ft, colors);

    if (FT_Init_FreeType(&lib)) {
        printl(LOG_W, "Error while initializing freetype library.\n");
        return 1;
    }
    if (FT_New_Face(lib, "fonts/inconsolata.ttf", 0, &face)) {
        printl(LOG_W, "Error while initializing freetype library: cannot load font.\n");
        return 1;
    }
    FT_Set_Pixel_Sizes(face, 0, ft_font_size);
    FT_GlyphSlot g = face->glyph;

    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(128, textures);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (int i = 32; i < 128; ++i) {
        if (FT_Load_Glyph(face, FT_Get_Char_Index(face, i), FT_LOAD_RENDER)) {
            printl(LOG_W, "Error while initializing freetype library: cannot load character %#x(%c).\n", i, (char) i);
            return 1;
        }
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, g->bitmap.width, g->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);
        
        ft_chars[i].size_x = g->bitmap.width;
        ft_chars[i].size_y = g->bitmap.rows;
        ft_chars[i].bearing_x = g->bitmap_left;
        ft_chars[i].bearing_y = g->bitmap_top;
        ft_chars[i].advance = g->advance.x >> 6;
        ft_chars[i].texcoord_x = 0;
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    glGenBuffers(1, &ft_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, ft_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ft_vbo_data), ft_vbo_data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    ft_mat_p = make_mat4();

    return 0;
}

void render_text(int x, int y, const char *str) {
    id_mat4(ft_mat_p);
    vec3f v = make_vec3(2.0 / window_width, 2.0 / window_height, 1);
    iscale_mat(v, ft_mat_p);
    free(v);
    v = make_vec3(-1, -1, 0);
    itrans_mat(v, ft_mat_p);
    free(v);
    glUseProgram(prog_ft);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUniformMatrix4fv(prog_ft_unif_mat_p, 1, GL_FALSE, ft_mat_p);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo_pp_texture);
    glUniform1i(prog_ft_unif_colors, 0);
    glActiveTexture(GL_TEXTURE1);
    glUniform1i(prog_ft_unif_texture, 1);
    glBindBuffer(GL_ARRAY_BUFFER, ft_vbo);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    for (const char *c = str; *c; ++c) {
        int i = *c;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        int x_pos1 = x + ft_chars[i].bearing_x;
        int y_pos1 = y - ft_chars[i].size_y + ft_chars[i].bearing_y;
        int x_pos2 = x + ft_chars[i].bearing_x + ft_chars[i].size_x;
        int y_pos2 = y - ft_chars[i].size_y + ft_chars[i].bearing_y + ft_chars[i].size_y;
        int x_tex1 = 0;
        int y_tex1 = 1;
        int x_tex2 = 1;
        int y_tex2 = 0;
        ft_vbo_data[0].coord[0] = x_pos1;
        ft_vbo_data[0].coord[1] = y_pos1;
        ft_vbo_data[0].texcoord[0] = x_tex1;
        ft_vbo_data[0].texcoord[1] = y_tex1;

        ft_vbo_data[1].coord[0] = x_pos2;
        ft_vbo_data[1].coord[1] = y_pos1;
        ft_vbo_data[1].texcoord[0] = x_tex2;
        ft_vbo_data[1].texcoord[1] = y_tex1;

        ft_vbo_data[2].coord[0] = x_pos2;
        ft_vbo_data[2].coord[1] = y_pos2;
        ft_vbo_data[2].texcoord[0] = x_tex2;
        ft_vbo_data[2].texcoord[1] = y_tex2;

        ft_vbo_data[3].coord[0] = x_pos1;
        ft_vbo_data[3].coord[1] = y_pos2;
        ft_vbo_data[3].texcoord[0] = x_tex1;
        ft_vbo_data[3].texcoord[1] = y_tex2;

        glEnableVertexAttribArray(prog_ft_attr_v_coord);
        glEnableVertexAttribArray(prog_ft_attr_v_texcoord);

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ft_vbo_data), ft_vbo_data);
        glVertexAttribPointer(prog_ft_attr_v_coord, 3, GL_FLOAT, GL_FALSE, sizeof(vertex3d), (void *) offsetof(vertex3d, coord));
        glVertexAttribPointer(prog_ft_attr_v_texcoord, 2, GL_FLOAT, GL_FALSE, sizeof(vertex3d), (void *) offsetof(vertex3d, texcoord));
        glDrawArrays(GL_QUADS, 0, 4);

        glDisableVertexAttribArray(prog_ft_attr_v_coord);
        glDisableVertexAttribArray(prog_ft_attr_v_texcoord);

        x += ft_chars[i].advance;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

void free_ft(void) {
    glDeleteProgram(prog_ft);
    glDeleteTextures(128, textures);
    glDeleteBuffers(1, &ft_vbo);
}
