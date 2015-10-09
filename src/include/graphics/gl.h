#ifndef GRAPHICS_GL_H
#define GRAPHICS_GL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math/vecmath.h>
#include <graphics/draw_obj.h>
#include <util/log.h>
#include <graphics/shader.h>
#include <graphics/vertex3d.h>

#include <GL/glew.h>
#include <GL/gl.h>

#define LIGHT_COUNT 8

#define INIT_ATTR(PROG, ATTR) do { \
    PROG##_attr_##ATTR = glGetAttribLocation(PROG, #ATTR); \
    if (PROG##_attr_##ATTR == -1U) { \
        printl(LOG_W, "Error while initializing gl: cannot bind attribute " #ATTR " (" #PROG ")\n"); \
        return 1; \
    } \
} while (0)

#define INIT_UNIF(PROG, UNIF) do { \
    PROG##_unif_##UNIF = glGetUniformLocation(PROG, #UNIF); \
    if (PROG##_unif_##UNIF == -1U) { \
        printl(LOG_W, "Error while initializing gl: cannot bind uniform " #UNIF " (" #PROG ")\n"); \
        return 1; \
    } \
} while (0)
#define INIT_PROG(NAME) do { \
    unsigned int vs = create_shader("shaders/" #NAME ".v.glsl", GL_VERTEX_SHADER); \
    if (vs == -1U) { \
        printl(LOG_W, "Error while initializing gl: cannot compile vertex " #NAME "shader.\n"); \
        return 1; \
    } \
    unsigned int fs = create_shader("shaders/" #NAME ".f.glsl", GL_FRAGMENT_SHADER); \
    if (fs == -1U) { \
        printl(LOG_W, "Error while initializing gl: cannot compile fragment " #NAME " shader.\n"); \
        return 1; \
    } \
    prog_##NAME = create_program(vs, fs); \
    if (prog_##NAME == -1U) { \
        printl(LOG_W, "Error while initializing gl: cannot compile " #NAME " program.\n"); \
        return 1; \
    } \
} while(0)


typedef struct {
    vec3f pos;
    vec3f rot;
    vec3f ambient;
    vec3f diffuse;
    vec3f specular;
    float fov;
    float z_near;
    float z_far;
} light_t;

extern light_t gl_light[LIGHT_COUNT];
extern char gl_light_enable[LIGHT_COUNT];
extern vec3f gl_pos, gl_rot;
extern float gl_fov, gl_z_near, gl_z_far;
extern mat4f mat_p, mat_v;
extern mat4f light_mat_p[LIGHT_COUNT], light_mat_v[LIGHT_COUNT];

int init_gl(void);
void gl_on_display(int n, draw_obj *count);
void gl_matrices(void);
void gl_light_matrices(void);
void gl_reshape(void);
void free_gl(void);

#ifdef __cplusplus
}
#endif

#endif // GRAPHICS_GL_H
