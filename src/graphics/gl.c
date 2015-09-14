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

char gl_light_enable[LIGHT_COUNT];
light_t gl_light[LIGHT_COUNT];
vec3f gl_pos, gl_rot;
float gl_fov, gl_z_near, gl_z_far;

mat4f mat_p, mat_v;
mat4f light_mat_p[LIGHT_COUNT], light_mat_v[LIGHT_COUNT];

unsigned int prog_depth, prog, prog_pp;
unsigned int fbo_depth[LIGHT_COUNT], fbo_depth_texture[LIGHT_COUNT];
unsigned int fbo_pp, rbo_pp, fbo_pp_texture;

unsigned int prog_depth_attr_v_coord;
unsigned int prog_depth_unif_mat_m, prog_depth_unif_mat_v, prog_depth_unif_mat_p;

unsigned int prog_attr_v_coord, prog_attr_v_normal;
unsigned int prog_unif_z_near, prog_unif_z_far;
unsigned int prog_unif_mat_m, prog_unif_mat_v, prog_unif_mat_p;

unsigned int prog_pp_attr_v_coord;
unsigned int prog_pp_unif_texture;

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

void matrices() {
    persp_mat(gl_fov, ((double) window_width) / ((double) window_height), gl_z_near, gl_z_far, mat_p);
    id_mat4(mat_v);
    gl_pos[0] *= -1;
    gl_pos[1] *= -1;
    gl_pos[2] *= -1;
    itrans_mat(gl_pos, mat_v);
    gl_pos[0] *= -1;
    gl_pos[1] *= -1;
    gl_pos[2] *= -1;
    irot_y_mat(gl_rot[1], mat_v);
    irot_x_mat(gl_rot[0], mat_v);
    for (int i = 0; i < LIGHT_COUNT; ++i) {
        if (gl_light_enable[i]) {
            persp_mat(gl_light[i].fov, 1, gl_light[i].z_near, gl_light[i].z_far, light_mat_p[i]);
            id_mat4(light_mat_v[i]);
            gl_light[i].pos[0] *= -1;
            gl_light[i].pos[1] *= -1;
            gl_light[i].pos[2] *= -1;
            itrans_mat(gl_light[i].pos, light_mat_v[i]);
            gl_light[i].pos[0] *= -1;
            gl_light[i].pos[1] *= -1;
            gl_light[i].pos[2] *= -1;
            irot_y_mat(gl_light[i].rot[1], light_mat_v[i]);
            irot_x_mat(gl_light[i].rot[0], light_mat_v[i]);
        }
    }
}

void prog_depth_uniforms(int i) {
    glUniformMatrix4fv(prog_depth_unif_mat_v, 1, GL_FALSE, light_mat_v[i]);
    glUniformMatrix4fv(prog_depth_unif_mat_p, 1, GL_FALSE, light_mat_p[i]);
}

void prog_depth_uniforms_obj(draw_obj_t obj) {
    glUniformMatrix4fv(prog_depth_unif_mat_m, 1, GL_FALSE, obj.mat_m);
}

void prog_uniforms(void) {
    char unif_name[100];
    glUniformMatrix4fv(prog_unif_mat_v, 1, GL_FALSE, mat_v);
    glUniformMatrix4fv(prog_unif_mat_p, 1, GL_FALSE, mat_p);
    glUniform1f(prog_unif_z_near, gl_z_near);
    glUniform1f(prog_unif_z_far, gl_z_far);
    for (int i = 0; i < LIGHT_COUNT; ++i) {
        sprintf(unif_name, "light_enable[%d]", i);
        glUniform1i(glGetUniformLocation(prog, unif_name), gl_light_enable[i]);
        if (gl_light_enable[i]) {
            sprintf(unif_name, "light[%d].coord", i);
            glUniform3fv(glGetUniformLocation(prog, unif_name), 1, gl_light[i].pos);
            sprintf(unif_name, "light[%d].ambient", i);
            glUniform3fv(glGetUniformLocation(prog, unif_name), 1, gl_light[i].ambient);
            sprintf(unif_name, "light[%d].diffuse", i);
            glUniform3fv(glGetUniformLocation(prog, unif_name), 1, gl_light[i].diffuse);
            sprintf(unif_name, "light[%d].specular", i);
            glUniform3fv(glGetUniformLocation(prog, unif_name), 1, gl_light[i].specular);
            sprintf(unif_name, "light[%d].mat_v", i);
            glUniformMatrix4fv(glGetUniformLocation(prog, unif_name), 1, GL_FALSE, light_mat_v[i]);
            sprintf(unif_name, "light[%d].mat_p", i);
            glUniformMatrix4fv(glGetUniformLocation(prog, unif_name), 1, GL_FALSE, light_mat_p[i]);
            sprintf(unif_name, "light[%d].z_near", i);
            glUniform1f(glGetUniformLocation(prog, unif_name), gl_light[i].z_near);
            sprintf(unif_name, "light[%d].z_far", i);
            glUniform1f(glGetUniformLocation(prog, unif_name), gl_light[i].z_far);
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, fbo_depth_texture[i]);
            sprintf(unif_name, "light[%d].map", i);
            glUniform1i(glGetUniformLocation(prog, unif_name), i);
        }
    }
}

void prog_uniforms_obj(draw_obj_t obj) {
    glUniformMatrix4fv(prog_unif_mat_m, 1, GL_FALSE, obj.mat_m);
    glUniform3f(glGetUniformLocation(prog, "material.ambient"), obj.material.ambient[0], obj.material.ambient[1], obj.material.ambient[2]);
    glUniform3fv(glGetUniformLocation(prog, "material.diffuse"), 1, obj.material.diffuse);
    glUniform3fv(glGetUniformLocation(prog, "material.specular"), 1, obj.material.specular);
    glUniform1f(glGetUniformLocation(prog, "material.shininess"), obj.material.shininess);
}

void prog_pp_uniforms(void) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo_pp_texture);
    glUniform1i(prog_pp_unif_texture, 0);
}

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

    mat_p = make_mat4();
    mat_v = make_mat4();
    for (int i = 0; i < LIGHT_COUNT; ++i) {
        light_mat_p[i] = make_mat4();
        light_mat_v[i] = make_mat4();
    }

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

    INIT_UNIF(prog_depth, mat_m);
    INIT_UNIF(prog_depth, mat_v);
    INIT_UNIF(prog_depth, mat_p);

    INIT_ATTR(prog, v_coord);
    INIT_ATTR(prog, v_normal);

    INIT_UNIF(prog, mat_m);
    INIT_UNIF(prog, mat_v);
    INIT_UNIF(prog, mat_p);
    INIT_UNIF(prog, z_near);
    INIT_UNIF(prog, z_far);

    INIT_ATTR(prog_pp, v_coord);
    INIT_UNIF(prog_pp, texture);

    glGenFramebuffers(LIGHT_COUNT, fbo_depth);
    glGenTextures(LIGHT_COUNT, fbo_depth_texture);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glActiveTexture(GL_TEXTURE0);
    for (int i = 0; i < LIGHT_COUNT; ++i) {
        glBindTexture(GL_TEXTURE_2D, fbo_depth_texture[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, fbo_depth[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fbo_depth_texture[i], 0);

        unsigned int depth_framebuffer_status;
        if ((depth_framebuffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
            printl(LOG_E, "Error while initializing resources: depth framebuffer %d is not complete (%d).\n", i, depth_framebuffer_status);
            return -1;
        }
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

void gl_reshape(void) {
    persp_mat(1, ((double) window_width) / ((double) window_height), .1, 100, mat_p);

    glBindTexture(GL_TEXTURE_2D, fbo_pp_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window_width, window_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, rbo_pp);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, window_width, window_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo_pp);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_pp_texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_pp);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void gl_on_display(int n, draw_obj_t *objs) {
    matrices();
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
  
    glUseProgram(prog);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDrawBuffer(GL_BACK);
    glReadBuffer(GL_BACK);
    glViewport(0, 0, window_width, window_height);
    glClearColor(0, 0, 0, 1);
    glClearDepth(1);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glCullFace(GL_BACK);
    glEnable(GL_TEXTURE_2D);

    prog_uniforms();

    glEnableVertexAttribArray(prog_attr_v_coord);
    glEnableVertexAttribArray(prog_attr_v_normal);

    for (int i = 0; i < n; ++i) {
        prog_uniforms_obj(objs[i]);

        glBindBuffer(GL_ARRAY_BUFFER, objs[i].vbo);
        glVertexAttribPointer(prog_attr_v_coord, 3, GL_FLOAT, GL_FALSE, sizeof(vertex3d), (void *) offsetof(vertex3d, coord));
        glVertexAttribPointer(prog_attr_v_normal, 3, GL_FLOAT, GL_FALSE, sizeof(vertex3d), (void *) offsetof(vertex3d, normal));
        if (objs[i].ibo) {
            glDrawElements(objs[i].mode, objs[i].count, GL_UNSIGNED_SHORT, objs[i].ibo);
        } else {
            glDrawArrays(objs[i].mode, 0, objs[i].count);
        }
    }
/*
    glDisableVertexAttribArray(prog_attr_v_coord);
    glDisableVertexAttribArray(prog_attr_v_normal);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glUseProgram(prog_pp);
    glClearColor(0, 0, 0, 1);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    prog_pp_uniforms();

    glEnableVertexAttribArray(prog_pp_attr_v_coord);

    glBindBuffer(GL_ARRAY_BUFFER, pp_vbo);
    glVertexAttribPointer(prog_pp_attr_v_coord, 3, GL_FLOAT, GL_FALSE, sizeof(vertex3d), (void*) offsetof(vertex3d, coord));

    glDrawArrays(GL_QUADS, 0, 4);

    glDisableVertexAttribArray(prog_pp_attr_v_coord);
*/
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
