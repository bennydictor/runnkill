#include <graphics/gl/pp.h>
#include <graphics/gl.h>
#include <graphics/glfw.h>
#include <graphics/objects/pp.h>

unsigned int prog_pp;
unsigned int fbo_pp, rbo_pp, fbo_pp_texture;

unsigned int prog_pp_attr_v_coord;
unsigned int prog_pp_unif_texture;

void prog_pp_uniforms(void) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo_pp_texture);
    glUniform1i(prog_pp_unif_texture, 0);
}

int init_gl_pp(void) {
    INIT_PROG(pp);

    INIT_ATTR(prog_pp, v_coord);
    INIT_UNIF(prog_pp, texture);

    glGenFramebuffers(1, &fbo_pp);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_pp);

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &fbo_pp_texture);
    glBindTexture(GL_TEXTURE_2D, fbo_pp_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window_width, window_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenRenderbuffers(1, &rbo_pp);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_pp);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, window_width, window_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glDrawBuffer(GL_BACK);
    glReadBuffer(GL_BACK);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_pp_texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_pp);

    unsigned int pp_framebuffer_status;
    if ((pp_framebuffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
        printl(LOG_W, "Error while initializing gl: postprocess framebuffer is not complete (%d).\n", pp_framebuffer_status);
        return 1;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return 0;
}

void render_pp(void) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glUseProgram(prog_pp);
    glDrawBuffer(GL_BACK);
    glReadBuffer(GL_BACK);
    glClearColor(0, 0, 0, 1);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    prog_pp_uniforms();

    glEnableVertexAttribArray(prog_pp_attr_v_coord);

    glBindBuffer(GL_ARRAY_BUFFER, pp_vbo);
    glVertexAttribPointer(prog_pp_attr_v_coord, 3, GL_FLOAT, GL_FALSE, sizeof(vertex3d), (void*) offsetof(vertex3d, coord));

    glDrawArrays(GL_QUADS, 0, 4);

    glDisableVertexAttribArray(prog_pp_attr_v_coord);
}

void free_gl_pp(void) {
    glDeleteProgram(prog_pp);
    glDeleteTextures(1, &fbo_pp_texture);
    glDeleteRenderbuffers(1, &rbo_pp);
    glDeleteFramebuffers(1, &fbo_pp);
}
