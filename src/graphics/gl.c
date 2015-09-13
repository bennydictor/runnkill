#include <graphics/gl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include <graphics/shader.h>
#include <graphics/glfw.h>
#include <graphics/draw_obj_t.h>
#include <util/log.h>
#include <math/vecmath.h>
#include <math/constants.h>

#include <graphics/objects/sphere.h>
#include <graphics/objects/box.h>
#include <graphics/objects/pp.h>

#define unused(X) ((void) (X))

unsigned int prog_depth, prog, prog_pp;
unsigned int fbo_depth, rbo_depth, fbo_depth_texture;
unsigned int fbo_pp, rbo_pp, fbo_pp_texture;
mat4f mat_v, mat_p;
vec3f pos, rot;

mat4f light_mat_v, light_mat_p;
vec3f light_pos, light_rot;

unsigned int prog_depth_attr_v_coord;
unsigned int prog_depth_unif_v_mat_m, prog_depth_unif_v_mat_v, prog_depth_unif_v_mat_p;

unsigned int prog_attr_v_coord, prog_attr_v_normal;
unsigned int prog_unif_v_mat_m, prog_unif_v_mat_v, prog_unif_v_mat_p;

unsigned int prog_pp_attr_v_coord;
unsigned int prog_pp_unif_f_texture;

#define INIT_ATTR(PROG, ATTR) do { \
    PROG##_attr_##ATTR = glGetAttribLocation(PROG, #ATTR); \
    if (PROG##_attr_##ATTR == -1U) { \
        printl(LOG_E, "Error while initializing resources: cannot bind attribute " #ATTR " (" #PROG ")\n"); \
        return -1; \
    } \
} while (0)

#define INIT_UNIF(PROG, UNIF) do { \
    PROG##_unif_##UNIF = glGetUniformLocation(PROG, #UNIF); \
    if (PROG##_unif_##UNIF == -1U) { \
        printl(LOG_E, "Error while initializing resources: cannot bind uniform " #UNIF " (" #PROG ")\n"); \
        return -1; \
    } \
} while (0)

int init_gl(void) {
    GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK) {
        printl(LOG_E, "%s\n", glewGetErrorString(glew_status));
        glfwTerminate();
        return -1;
    }

    init_sphere_object();
    init_box_object();
    init_pp_object();

    mat_v = make_mat4();
    mat_p = make_mat4();
    persp_mat(1, ((double) window_width) / ((double) window_height), .1, 100, mat_p);
    pos = make_vec3(-5, 3, 5);
    rot = make_vec3(M_PI / 9, M_PI / 4, 0);

    light_mat_v = make_mat4();
    light_mat_p = make_mat4();
    persp_mat(1, ((double) window_width) / ((double) window_height), .1, 100, light_mat_p);
    light_pos = make_vec3(0, 5, 10);
    light_rot = make_vec3(M_PI / 8, 0, 0);

    unsigned int vs_depth = create_shader("shaders/depth.v.glsl", GL_VERTEX_SHADER);
    if (vs_depth == -1U) {
        printl(LOG_E, "Error while initializing resources: cannot compile vertex depth shader.\n");
        return -1;
    }

    unsigned int fs_depth = create_shader("shaders/depth.f.glsl", GL_FRAGMENT_SHADER);
    if (fs_depth == -1U) {
        printl(LOG_E, "Error while initializing resources: cannot compile fragment depth shader.\n");
        return -1;
    }

    prog_depth = create_program(vs_depth, fs_depth);
    if (prog_depth == -1U) {
        printl(LOG_E, "Error while initializing resources: cannot compile depth program.\n");
        return -1;
    }

    unsigned int vs = create_shader("shaders/light.v.glsl", GL_VERTEX_SHADER);
    if (vs == -1U) {
        printl(LOG_E, "Error while initializing resources: cannot compile vertex shader.\n");
        return -1;
    }

    unsigned int fs = create_shader("shaders/light.f.glsl", GL_FRAGMENT_SHADER);
    if (fs == -1U) {
        printl(LOG_E, "Error while initializing resources: cannot compile fragment shader.\n");
        return -1;
    }

    prog = create_program(vs, fs);
    if (prog == -1U) {
        printl(LOG_E, "Error while initializing resources: cannot compile program.\n");
        return -1;
    }

    unsigned int vs_pp = create_shader("shaders/postprocess.v.glsl", GL_VERTEX_SHADER);
    if (vs == -1U) {
        printl(LOG_E, "Error while initializing resources: cannot compile postprocess vertex shader.\n");
        return -1;
    }

    unsigned int fs_pp = create_shader("shaders/postprocess.f.glsl", GL_FRAGMENT_SHADER);
    if (fs == -1U) {
        printl(LOG_E, "Error while initializing resources: cannot compile postprocess fragment shader.\n");
        return -1;
    }

    prog_pp = create_program(vs_pp, fs_pp);
    if (prog == -1U) {
        printl(LOG_E, "Error while initializing resources: cannot compile postprocess program.\n");
        return -1;
    }

    INIT_ATTR(prog_depth, v_coord);

    INIT_UNIF(prog_depth, v_mat_m);
    INIT_UNIF(prog_depth, v_mat_v);
    INIT_UNIF(prog_depth, v_mat_p);

    INIT_ATTR(prog, v_coord);
    INIT_ATTR(prog, v_normal);

    INIT_UNIF(prog, v_mat_m);
    INIT_UNIF(prog, v_mat_v);
    INIT_UNIF(prog, v_mat_p);

    INIT_ATTR(prog_pp, v_coord);
    INIT_UNIF(prog_pp, f_texture);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glUseProgram(prog);

    glUniform3f(glGetUniformLocation(prog, "f_material.ambient"), 1, 1, 1);
    glUniform3f(glGetUniformLocation(prog, "f_material.diffuse"), 1, 1, 1);
    glUniform3f(glGetUniformLocation(prog, "f_material.specular"), 1, 1, 1);
    glUniform1f(glGetUniformLocation(prog, "f_material.shininess"), 128);

    glUniform3f(glGetUniformLocation(prog, "f_light[0].coord"), light_pos[0], light_pos[1], light_pos[2]);
    glUniform3f(glGetUniformLocation(prog, "f_light[0].ambient"), .2, .2, .2);
    glUniform3f(glGetUniformLocation(prog, "f_light[0].diffuse"), .9, .9, .9);
    glUniform3f(glGetUniformLocation(prog, "f_light[0].specular"), 1, 1, 1);

    glUniform1i(glGetUniformLocation(prog, "f_light_enable[0]"), 1);
    glUniform1i(glGetUniformLocation(prog, "f_light_enable[1]"), 0);
    glUniform1i(glGetUniformLocation(prog, "f_light_enable[2]"), 0);
    glUniform1i(glGetUniformLocation(prog, "f_light_enable[3]"), 0);
    glUniform1i(glGetUniformLocation(prog, "f_light_enable[4]"), 0);
    glUniform1i(glGetUniformLocation(prog, "f_light_enable[5]"), 0);
    glUniform1i(glGetUniformLocation(prog, "f_light_enable[6]"), 0);
    glUniform1i(glGetUniformLocation(prog, "f_light_enable[7]"), 0);

    glGenFramebuffers(1, &fbo_depth);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_depth);

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &fbo_depth_texture);
    glBindTexture(GL_TEXTURE_2D, fbo_depth_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fbo_depth_texture, 0);

    unsigned int depth_framebuffer_status;
    if ((depth_framebuffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
        printl(LOG_E, "Error while initializing resources: depth framebuffer is not complete (%d).\n", depth_framebuffer_status);
        return -1;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
        printl(LOG_E, "Error while initializing resources: postprocess framebuffer is not complete (%d).\n", pp_framebuffer_status);
        return -1;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return 0;
}

void gl_on_display(int n, draw_obj_t *objs) {
    glUseProgram(prog_depth);
    glUniformMatrix4fv(prog_depth_unif_v_mat_v, 1, GL_FALSE, light_mat_v);
    glUniformMatrix4fv(prog_depth_unif_v_mat_p, 1, GL_FALSE, light_mat_p);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo_depth);
    glViewport(0, 0, 1024, 1024);
    glClearColor(0, 0, 0, 1);
    glClearDepth(1);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDrawBuffer(GL_NONE);

    glEnableVertexAttribArray(prog_depth_attr_v_coord);

    for (int i = 0; i < n; ++i) {
        glUniformMatrix4fv(prog_depth_unif_v_mat_m, 1, GL_FALSE, objs[i].mat_m);
        glBindBuffer(GL_ARRAY_BUFFER, objs[i].vbo);
        glVertexAttribPointer(prog_depth_attr_v_coord, 3, GL_FLOAT, GL_FALSE, sizeof(vertex3d), (void *) offsetof(vertex3d, coord));
        if (objs[i].ibo) {
            glDrawElements(objs[i].mode, objs[i].count, GL_UNSIGNED_SHORT, objs[i].ibo);
        } else {
            glDrawArrays(objs[i].mode, 0, objs[i].count);
        }
    }

    glDisableVertexAttribArray(prog_depth_attr_v_coord);
  
    glUseProgram(prog);
    glUniformMatrix4fv(prog_unif_v_mat_v, 1, GL_FALSE, mat_v);
    glUniformMatrix4fv(prog_unif_v_mat_p, 1, GL_FALSE, mat_p);
    glUniformMatrix4fv(glGetUniformLocation(prog, "f_light[0].mat_v"), 1, GL_FALSE, light_mat_v);
    glUniformMatrix4fv(glGetUniformLocation(prog, "f_light[0].mat_p"), 1, GL_FALSE, light_mat_p);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo_pp);
    glViewport(0, 0, window_width, window_height);
    glClearColor(0, 0, 0, 1);
    glClearDepth(1);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glDrawBuffer(GL_BACK);

    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo_depth_texture);
    glUniform1i(glGetUniformLocation(prog, "f_light[0].map"), 0);

    glEnableVertexAttribArray(prog_attr_v_coord);
    glEnableVertexAttribArray(prog_attr_v_normal);

    for (int i = 0; i < n; ++i) {
        glUniformMatrix4fv(prog_unif_v_mat_m, 1, GL_FALSE, objs[i].mat_m);
        glBindBuffer(GL_ARRAY_BUFFER, objs[i].vbo);
        glVertexAttribPointer(prog_attr_v_coord, 3, GL_FLOAT, GL_FALSE, sizeof(vertex3d), (void *) offsetof(vertex3d, coord));
        glVertexAttribPointer(prog_attr_v_normal, 3, GL_FLOAT, GL_FALSE, sizeof(vertex3d), (void *) offsetof(vertex3d, normal));
        if (objs[i].ibo) {
            glDrawElements(objs[i].mode, objs[i].count, GL_UNSIGNED_SHORT, objs[i].ibo);
        } else {
            glDrawArrays(objs[i].mode, 0, objs[i].count);
        }
    }

    glDisableVertexAttribArray(prog_attr_v_coord);
    glDisableVertexAttribArray(prog_attr_v_normal);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glUseProgram(prog_pp);
    glClearColor(0, 0, 0, 1);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo_pp_texture);
    glUniform1i(prog_pp_unif_f_texture, 0);

    glEnableVertexAttribArray(prog_pp_attr_v_coord);

    glBindBuffer(GL_ARRAY_BUFFER, pp_vbo);
    glVertexAttribPointer(prog_pp_attr_v_coord, 3, GL_FLOAT, GL_FALSE, sizeof(vertex3d), (void*) offsetof(vertex3d, coord));

    glDrawArrays(GL_QUADS, 0, 4);

    glDisableVertexAttribArray(prog_pp_attr_v_coord);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void free_gl(void) {
    glDeleteProgram(prog);
    glDeleteProgram(prog_pp);
    free_sphere_object();
    free_box_object();
    free_pp_object();
    glDeleteTextures(1, &fbo_pp_texture);
    glDeleteRenderbuffers(1, &rbo_pp);
    glDeleteFramebuffers(1, &fbo_pp);
    free(mat_v);
    free(mat_p);
}
