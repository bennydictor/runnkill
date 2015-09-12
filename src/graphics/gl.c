#include <graphics/gl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include <graphics/shader.h>
#include <graphics/glfw.h>
#include <util/log.h>
#include <math/vecmath.h>
#include <math/constants.h>

#include <graphics/objects/sphere.h>
#include <graphics/objects/pp.h>

#define unused(X) ((void) (X))

unsigned int prog, prog_pp;
unsigned int vbo, vbo_pp;
unsigned int fbo, rbo, fbo_texture;
mat4f mat_m, mat_v, mat_p;
vec3f pos, rot;

unsigned int attr_v_coord, attr_v_normal;
unsigned int unif_v_mat_m, unif_v_mat_v, unif_v_mat_p;

unsigned int attr_pp_v_coord;
unsigned int unif_pp_f_texture;

#define INIT_ATTR(ATTR) do { \
    attr_##ATTR = glGetAttribLocation(prog, #ATTR); \
    if (attr_##ATTR == -1U) { \
        printl(LOG_E, "Error while initializing resources: cannot bind attribute " #ATTR "\n"); \
        return -1; \
    } \
} while (0)
#define INIT_UNIF(UNIF) do { \
    unif_##UNIF = glGetUniformLocation(prog, #UNIF); \
    if (unif_##UNIF == -1U) { \
        printl(LOG_E, "Error while initializing resources: cannot bind uniform " #UNIF "\n"); \
        return -1; \
    } \
} while (0)

#define INIT_ATTR_PP(ATTR) do { \
    attr_pp_##ATTR = glGetAttribLocation(prog_pp, #ATTR); \
    if (attr_pp_##ATTR == -1U) { \
        printl(LOG_E, "Error while initializing resources: cannot bind attribute " #ATTR " (postprocess)\n"); \
        return -1; \
    } \
} while (0)
#define INIT_UNIF_PP(UNIF) do { \
    unif_pp_##UNIF = glGetUniformLocation(prog_pp, #UNIF); \
    if (unif_pp_##UNIF == -1U) { \
        printl(LOG_E, "Error while initializing resources: cannot bind uniform " #UNIF " (postprocess)\n"); \
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

    init_sphere();
    mat_m = make_mat4();
    mat_v = make_mat4();
    mat_p = make_mat4();
    id_mat4(mat_m);
    id_mat4(mat_v);
    id_mat4(mat_p);
    persp_mat(1, ((double) window_width) / ((double) window_height), .0001, 1000, mat_p);
    pos = make_vec3(0, 0, -5);
    rot = make_vec3(0, 0, 0);
    trans_mat(pos, mat_v);

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

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vbo_data), sphere_vbo_data, GL_STATIC_DRAW);

    glGenBuffers(1, &vbo_pp);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_pp);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pp_vbo_data), pp_vbo_data, GL_STATIC_DRAW);

    INIT_ATTR(v_coord);
    INIT_ATTR(v_normal);

    INIT_UNIF(v_mat_m);
    INIT_UNIF(v_mat_v);
    INIT_UNIF(v_mat_p);

    INIT_ATTR_PP(v_coord);
    INIT_UNIF_PP(f_texture);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glUseProgram(prog);

    glUniform3f(glGetUniformLocation(prog, "f_material.ambient"), 1, 1, 1);
    glUniform3f(glGetUniformLocation(prog, "f_material.diffuse"), 1, 1, 1);
    glUniform3f(glGetUniformLocation(prog, "f_material.specular"), 0, 0, 0);
    glUniform1f(glGetUniformLocation(prog, "f_material.shininess"), 128);

    glUniform3f(glGetUniformLocation(prog, "f_light[0].coord"), 0, 3, 2);
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

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &fbo_texture);
    glBindTexture(GL_TEXTURE_2D, fbo_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window_width, window_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, window_width, window_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

    unsigned int framebuffer_status;
    if ((framebuffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
        printl(LOG_E, "Error while initializing resources: framebuffer is not complete (%d).\n", framebuffer_status);

    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return 0;
}

void gl_on_display(void) {
    glUseProgram(prog);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClearColor(0, 0, 0, 1);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUniformMatrix4fv(unif_v_mat_m, 1, GL_FALSE, mat_m);
    glUniformMatrix4fv(unif_v_mat_v, 1, GL_FALSE, mat_v);
    glUniformMatrix4fv(unif_v_mat_p, 1, GL_FALSE, mat_p);

    glEnableVertexAttribArray(attr_v_coord);
    glEnableVertexAttribArray(attr_v_normal);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(attr_v_coord, 3, GL_FLOAT, GL_FALSE, sizeof(vertex3d), (void *) offsetof(vertex3d, coord));
    glVertexAttribPointer(attr_v_normal, 3, GL_FLOAT, GL_FALSE, sizeof(vertex3d), (void *) offsetof(vertex3d, normal));

    glDrawElements(GL_QUADS, sizeof(sphere_ibo_data) / sizeof(short int), GL_UNSIGNED_SHORT, sphere_ibo_data);

    glDisableVertexAttribArray(attr_v_coord);
    glDisableVertexAttribArray(attr_v_normal);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glUseProgram(prog_pp);
    glClearColor(0, 0, 0, 1);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, fbo_texture);
    glUniform1i(unif_pp_f_texture, /*GL_TEXTURE*/0);

    glEnableVertexAttribArray(attr_pp_v_coord);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_pp);
    glVertexAttribPointer(attr_pp_v_coord, 3, GL_FLOAT, GL_FALSE, sizeof(vertex3d), (void*) offsetof(vertex3d, coord));

    glDrawArrays(GL_QUADS, 0, 4);

    glDisableVertexAttribArray(attr_pp_v_coord);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void free_gl(void) {
    glDeleteProgram(prog);
    glDeleteBuffers(1, &vbo);
    glDeleteTextures(1, &fbo_texture);
    glDeleteRenderbuffers(1, &rbo);
    glDeleteFramebuffers(1, &fbo);
    free(mat_m);
    free(mat_v);
    free(mat_p);
}
