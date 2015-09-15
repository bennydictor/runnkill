#include <graphics/gl/light.h>
#include <graphics/gl/depth.h>
#include <graphics/gl/pp.h>
#include <graphics/gl.h>
#include <graphics/glfw.h>

unsigned int prog_light;

unsigned int prog_light_attr_v_coord, prog_light_attr_v_normal;
unsigned int prog_light_unif_z_near, prog_light_unif_z_far;
unsigned int prog_light_unif_mat_m, prog_light_unif_mat_v, prog_light_unif_mat_p;

void prog_light_uniforms(void) {
    char unif_name[100];
    glUniformMatrix4fv(prog_light_unif_mat_v, 1, GL_FALSE, mat_v);
    glUniformMatrix4fv(prog_light_unif_mat_p, 1, GL_FALSE, mat_p);
    glUniform1f(prog_light_unif_z_near, gl_z_near);
    glUniform1f(prog_light_unif_z_far, gl_z_far);
    for (int i = 0; i < LIGHT_COUNT; ++i) {
        sprintf(unif_name, "light_enable[%d]", i);
        glUniform1i(glGetUniformLocation(prog_light, unif_name), gl_light_enable[i]);
        if (gl_light_enable[i]) {
            sprintf(unif_name, "light[%d].coord", i);
            glUniform3fv(glGetUniformLocation(prog_light, unif_name), 1, gl_light[i].pos);
            sprintf(unif_name, "light[%d].ambient", i);
            glUniform3fv(glGetUniformLocation(prog_light, unif_name), 1, gl_light[i].ambient);
            sprintf(unif_name, "light[%d].diffuse", i);
            glUniform3fv(glGetUniformLocation(prog_light, unif_name), 1, gl_light[i].diffuse);
            sprintf(unif_name, "light[%d].specular", i);
            glUniform3fv(glGetUniformLocation(prog_light, unif_name), 1, gl_light[i].specular);
            sprintf(unif_name, "light[%d].mat_v", i);
            glUniformMatrix4fv(glGetUniformLocation(prog_light, unif_name), 1, GL_FALSE, light_mat_v[i]);
            sprintf(unif_name, "light[%d].mat_p", i);
            glUniformMatrix4fv(glGetUniformLocation(prog_light, unif_name), 1, GL_FALSE, light_mat_p[i]);
            sprintf(unif_name, "light[%d].z_near", i);
            glUniform1f(glGetUniformLocation(prog_light, unif_name), gl_light[i].z_near);
            sprintf(unif_name, "light[%d].z_far", i);
            glUniform1f(glGetUniformLocation(prog_light, unif_name), gl_light[i].z_far);
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, fbo_depth_texture[i]);
            sprintf(unif_name, "light[%d].map", i);
            glUniform1i(glGetUniformLocation(prog_light, unif_name), i);
        }
    }
}

void prog_light_uniforms_obj(draw_obj_t obj) {
    glUniformMatrix4fv(prog_light_unif_mat_m, 1, GL_FALSE, obj.mat_m);
    glUniform3f(glGetUniformLocation(prog_light, "material.ambient"), obj.material.ambient[0], obj.material.ambient[1], obj.material.ambient[2]);
    glUniform3fv(glGetUniformLocation(prog_light, "material.diffuse"), 1, obj.material.diffuse);
    glUniform3fv(glGetUniformLocation(prog_light, "material.specular"), 1, obj.material.specular);
    glUniform1f(glGetUniformLocation(prog_light, "material.shininess"), obj.material.shininess);
}

int init_gl_light(void) {
    INIT_PROG(light);

    INIT_ATTR(prog_light, v_coord);
    INIT_ATTR(prog_light, v_normal);

    INIT_UNIF(prog_light, mat_m);
    INIT_UNIF(prog_light, mat_v);
    INIT_UNIF(prog_light, mat_p);
    INIT_UNIF(prog_light, z_near);
    INIT_UNIF(prog_light, z_far);

    return 0;
}

void render_light(int n, draw_obj_t *objs) {
    glUseProgram(prog_light);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo_pp);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawBuffer(GL_BACK);
    glReadBuffer(GL_BACK);
    glViewport(0, 0, window_width, window_height);
    glClearColor(0, 0, 0, 1);
    glClearDepth(1);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glCullFace(GL_BACK);
    glEnable(GL_TEXTURE_2D);

    prog_light_uniforms();

    glEnableVertexAttribArray(prog_light_attr_v_coord);
    glEnableVertexAttribArray(prog_light_attr_v_normal);

    for (int i = 0; i < n; ++i) {
        prog_light_uniforms_obj(objs[i]);

        glBindBuffer(GL_ARRAY_BUFFER, objs[i].vbo);
        glVertexAttribPointer(prog_light_attr_v_coord, 3, GL_FLOAT, GL_FALSE, sizeof(vertex3d), (void *) offsetof(vertex3d, coord));
        glVertexAttribPointer(prog_light_attr_v_normal, 3, GL_FLOAT, GL_FALSE, sizeof(vertex3d), (void *) offsetof(vertex3d, normal));
        if (objs[i].ibo) {
            glDrawElements(objs[i].mode, objs[i].count, GL_UNSIGNED_SHORT, objs[i].ibo);
        } else {
            glDrawArrays(objs[i].mode, 0, objs[i].count);
        }
    }

    glDisableVertexAttribArray(prog_light_attr_v_coord);
    glDisableVertexAttribArray(prog_light_attr_v_normal);
}

void free_gl_light(void) {
    glDeleteProgram(prog_light);
}
