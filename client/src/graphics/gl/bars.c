#include <graphics/gl/bars.h>
#include <graphics/gl.h>
#include <graphics/glfw.h>
#include <graphics/objects/rect.h>
#include <world.h>

unsigned int prog_bars;

unsigned int prog_bars_attr_v_coord;
unsigned int prog_bars_unif_mat_m, prog_bars_unif_mat_p;
unsigned int prog_bars_unif_color;

mat4f bars_mat_p;

int init_gl_bars(void) {
    INIT_PROG(bars);

    INIT_ATTR(prog_bars, v_coord);

    INIT_UNIF(prog_bars, mat_m);
    INIT_UNIF(prog_bars, mat_p);
    INIT_UNIF(prog_bars, color);

    bars_mat_p = make_mat4();

    return 0;
}

void render_bars(void) {
    id_mat4(bars_mat_p);
    vec3f v = make_vec3(2.0 / window_width, 2.0 / window_height, 1);
    iscale_mat(v, bars_mat_p);
    free(v);
    v = make_vec3(-1, -1, 0);
    itrans_mat(v, bars_mat_p);
    free(v);
    glUseProgram(prog_bars);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUniformMatrix4fv(prog_bars_unif_mat_p, 1, GL_FALSE, bars_mat_p);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);

    glEnableVertexAttribArray(prog_bars_attr_v_coord);

    draw_obj bar = make_draw_rect(window_width / 2 * (max_hp - hp) / max_hp, 0, window_width / 2, 20, default_material);
    glUniformMatrix4fv(prog_bars_unif_mat_m, 1, GL_FALSE, bar.mat_m);
    glUniform3f(prog_bars_unif_color, 1, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, bar.vbo);
    glVertexAttribPointer(prog_bars_attr_v_coord, 3, GL_FLOAT, GL_FALSE, sizeof(vertex3d), (void *) offsetof(vertex3d, coord));
    glDrawArrays(bar.mode, 0, bar.count);
    free_draw_obj(bar);

    bar = make_draw_rect(window_width / 2, 0, window_width - window_width / 2 * (max_mp - mp) / max_mp, 20, default_material);
    glUniformMatrix4fv(prog_bars_unif_mat_m, 1, GL_FALSE, bar.mat_m);
    glUniform3f(prog_bars_unif_color, 0, 0, 1);
    glBindBuffer(GL_ARRAY_BUFFER, bar.vbo);
    glVertexAttribPointer(prog_bars_attr_v_coord, 3, GL_FLOAT, GL_FALSE, sizeof(vertex3d), (void *) offsetof(vertex3d, coord));
    glDrawArrays(bar.mode, 0, bar.count);
    free_draw_obj(bar);

    glDisableVertexAttribArray(prog_bars_attr_v_coord);
}

void free_gl_bars(void) {
    glDeleteProgram(prog_bars);
}
