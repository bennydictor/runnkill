#include <graphics/gl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include <graphics/shader.h>
#include <graphics/glfw.h>
#include <graphics/draw_obj_t.h>
#include <math/vecmath.h>
#include <math/constants.h>
#include <util/timing.h>

#include <graphics/vertex3d.h>
#include <graphics/ft.h>
#include <graphics/objects/sphere.h>
#include <graphics/objects/box.h>
#include <graphics/objects/pp.h>

#include <graphics/gl/depth.h>
#include <graphics/gl/light.h>
#include <graphics/gl/pp.h>

#define unused(X) ((void) (X))

char gl_light_enable[LIGHT_COUNT];
light_t gl_light[LIGHT_COUNT];
vec3f gl_pos, gl_rot;
float gl_fov, gl_z_near, gl_z_far;

mat4f mat_p, mat_v;
mat4f light_mat_p[LIGHT_COUNT], light_mat_v[LIGHT_COUNT];

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

int init_gl(void) {
    GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK) {
        printl(LOG_W, "%s\n", glewGetErrorString(glew_status));
        glfwTerminate();
        return 1;
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

    if (init_gl_depth()) {
        printl(LOG_W, "Error while initializing gl: cannot initialize depth module.\n");
        return 1;
    }
    if (init_gl_light()) {
        printl(LOG_W, "Error while initializing gl: cannot initialize light module.\n");
        return 1;
    }
    if (init_gl_pp()) {
        printl(LOG_W, "Error while initializing gl: cannot initialize pp module.\n");
        return 1;
    }
    if (init_ft()) {
        printl(LOG_W, "Error while initializing gl: cannot initialize ft.\n");
        return 1;
    }

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

    render_depth(n, objs);
    render_light(n, objs);
    render_pp();
    char fps_string[20];
    sprintf(fps_string, "fps: %d", fps);
    render_text(10, window_height - ft_font_size - 10, fps_string);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void free_gl(void) {
    free_gl_depth();
    free_gl_light();
    free_gl_pp();
    free_ft();
    free_sphere_object();
    free_box_object();
    free_pp_object();
    free(mat_v);
    free(mat_p);
}
