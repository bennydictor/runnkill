#include <graphics/gl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include <graphics/shader.h>
#include <graphics/glfw.h>
#include <graphics/draw_obj.h>
#include <math/vecmath.h>
#include <math/constants.h>
#include <util/timing.h>

#include <graphics/vertex3d.h>
#include <graphics/ft.h>
#include <graphics/objects/field.h>
#include <graphics/objects/sphere.h>
#include <graphics/objects/sphere_sector.h>
#include <graphics/objects/box.h>
#include <graphics/objects/rect.h>
#include <graphics/objects/pp.h>

#include <graphics/gl/depth.h>
#include <graphics/gl/light.h>
#include <graphics/gl/pp.h>
#include <graphics/gl/bars.h>

#include <world.h>

#define unused(X) ((void) (X))

char gl_light_enable[LIGHT_COUNT];
light_t gl_light[LIGHT_COUNT];
vec3f gl_pos, gl_rot;
float gl_fov, gl_z_near, gl_z_far;

mat4f mat_p, mat_v;
mat4f light_mat_p[LIGHT_COUNT], light_mat_v[LIGHT_COUNT];

void gl_matrices(void) {
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
}

void gl_light_matrices(void) {
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

    init_field_object(world_w, world_h, world_field);
    init_sphere_object();
    init_sphere_sector_object();
    init_box_object();
    init_rect_object();
    init_pp_object();

    mat_p = make_mat4();
    mat_v = make_mat4();
    for (int i = 0; i < LIGHT_COUNT; ++i) {
        light_mat_p[i] = make_mat4();
        light_mat_v[i] = make_mat4();
    }

    if (init_material()) {
        printl(LOG_W, "Error while initializing gl: cannot initialize material module.\n");
        return 1;
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
    if (init_gl_bars()) {
        printl(LOG_W, "Error while initializing gl: cannot initialize bars module\n");
        return 1;
    }
    if (init_ft()) {
        printl(LOG_W, "Error while initializing gl: cannot initialize ft.\n");
        return 1;
    }
    gl_matrices();
    gl_light_matrices();

    return 0;
}

void gl_reshape(void) {
    persp_mat(1, ((double) window_width) / ((double) window_height), gl_z_near, gl_z_far, mat_p);

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

void gl_update(int n, draw_obj *objs) {
    //render_depth(n, objs);
    render_light(n, objs);
    render_pp();
    char fps_string[1024 * 1024];
    sprintf(fps_string, "fps: %d\ncoords: %f %f %f\nspeed: %f %f %f\norientation: %f %f %f\nhp: %f\nmp: %f\nmax_hp: %f\nmax_mp: %f\nagility: %f\nstrength: %f\nintellect: %f\nabs_speed: %f\njump_high: %f\n EP: %d; level: %d; next_level: %d\n",
            fps,
            coords[0], coords[1], coords[2],
            speed[0], speed[1], speed[2],
            orientation[0], orientation[1], orientation[2],
            hp, mp, max_hp, max_mp, agility, strength, intellect, abs_speed, jump_high,
            EP, level, level_exp
            );
    render_bars();
    render_text(10, window_height - ft_font_size - 50, fps_string);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void gl_callback(char *evs) {
    gl_rot[0] += cursor_dy / 100.0;
    gl_rot[1] += cursor_dx / 100.0;
    cursor_dx = cursor_dy = 0;
    if (gl_rot[0] < -M_PI / 2) {
        gl_rot[0] = -M_PI / 2;
    }
    if (gl_rot[0] > +M_PI / 2) {
        gl_rot[0] = +M_PI / 2;
    }
    if (gl_rot[1] < -2 * M_PI) {
        gl_rot[1] += 2 * M_PI;
    }
    if (gl_rot[1] > +2 * M_PI) {
        gl_rot[1] -= 2 * M_PI;
    }

    gl_matrices();
    evs[WORLD_MOVE_FORWARD_EVENT]   = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
    evs[WORLD_MOVE_RIGHT_EVENT]     = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
    evs[WORLD_MOVE_BACKWARD_EVENT]  = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
    evs[WORLD_MOVE_LEFT_EVENT]      = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
    evs[WORLD_ATTACK_EVENT]         = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
    evs[WORLD_SYM_1]                = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
    evs[WORLD_SYM_2]                = glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS;
    evs[WORLD_SYM_3]                = glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS;
    //evs[WORLD_SYM_4]                = glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS;

}


void free_gl(void) {
    free_gl_depth();
    free_gl_light();
    free_gl_pp();
    free_gl_bars();
    free_ft();
    free_sphere_object();
    free_sphere_sector_object();
    free_box_object();
    free_rect_object();
    free_pp_object();
    free(mat_v);
    free(mat_p);
    free(gl_pos);
    free(gl_rot);
    for (int i = 0; i < LIGHT_COUNT; ++i) {
        free(light_mat_v[i]);
        free(light_mat_p[i]);
    }
}
