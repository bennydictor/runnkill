#include <graphics/gl/depth.h>

unsigned int prog_depth;
unsigned int fbo_depth[LIGHT_COUNT], fbo_depth_texture[LIGHT_COUNT];

unsigned int prog_depth_attr_v_coord;
unsigned int prog_depth_unif_mat_m, prog_depth_unif_mat_v, prog_depth_unif_mat_p;

void prog_depth_uniforms(int i) {
    glUniformMatrix4fv(prog_depth_unif_mat_v, 1, GL_FALSE, light_mat_v[i]);
    glUniformMatrix4fv(prog_depth_unif_mat_p, 1, GL_FALSE, light_mat_p[i]);
}

void prog_depth_uniforms_obj(draw_obj_t obj) {
    glUniformMatrix4fv(prog_depth_unif_mat_m, 1, GL_FALSE, obj.mat_m);
}

int init_gl_depth(void) {
    INIT_PROG(depth);

    INIT_ATTR(prog_depth, v_coord);

    INIT_UNIF(prog_depth, mat_m);
    INIT_UNIF(prog_depth, mat_v);
    INIT_UNIF(prog_depth, mat_p);

    glGenFramebuffers(LIGHT_COUNT, fbo_depth);
    glGenTextures(LIGHT_COUNT, fbo_depth_texture);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glActiveTexture(GL_TEXTURE0);
    for (int i = 0; i < LIGHT_COUNT; ++i) {
        glBindTexture(GL_TEXTURE_2D, fbo_depth_texture[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, fbo_depth[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fbo_depth_texture[i], 0);

        unsigned int depth_framebuffer_status;
        if ((depth_framebuffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
            printl(LOG_W, "Error while initializing gl: depth framebuffer %d is not complete (%d).\n", i, depth_framebuffer_status);
            return 1;
        }
    }

    return 0;
}

void render_depth(int n, draw_obj_t *objs) {
    glUseProgram(prog_depth);

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glCullFace(GL_FRONT);
    glEnableVertexAttribArray(prog_depth_attr_v_coord);
    for (int i = 0; i < LIGHT_COUNT; ++i) {
        if (gl_light_enable[i]) {
            glBindFramebuffer(GL_FRAMEBUFFER, fbo_depth[i]);
            glViewport(0, 0, 1024, 1024);
            glClearColor(0, 0, 0, 1);
            glClearDepth(1);
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

            prog_depth_uniforms(i);

            for (int j = 0; j < n; ++j) {
                prog_depth_uniforms_obj(objs[j]);
                glBindBuffer(GL_ARRAY_BUFFER, objs[j].vbo);
                glVertexAttribPointer(prog_depth_attr_v_coord, 3, GL_FLOAT, GL_FALSE, sizeof(vertex3d), (void *) offsetof(vertex3d, coord));
                if (objs[j].ibo) {
                    glDrawElements(objs[j].mode, objs[j].count, GL_UNSIGNED_SHORT, objs[j].ibo);
                } else {
                    glDrawArrays(objs[j].mode, 0, objs[j].count);
                }
            }
        }
    }
    glDisableVertexAttribArray(prog_depth_attr_v_coord);
}

void free_gl_depth(void) {
    glDeleteProgram(prog_depth);
    glDeleteFramebuffers(LIGHT_COUNT, fbo_depth);
    glDeleteTextures(LIGHT_COUNT, fbo_depth_texture);
}
