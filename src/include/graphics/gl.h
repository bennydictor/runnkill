#ifndef GRAPHICS_GL_H
#define GRAPHICS_GL_H

#include <math/vecmath.h>
#include <graphics/draw_obj_t.h>

#include <GL/glew.h>
#include <GL/gl.h>

#define LIGHT_COUNT 8


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

int init_gl(void);
void gl_on_display(int n, draw_obj_t *count);
void gl_reshape(void);
void free_gl(void);


#endif // GRAPHICS_GL_H
