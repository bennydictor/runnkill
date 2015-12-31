#ifndef GRAPHICS_GLFW_H
#define GRAPHICS_GLFW_H

#include <graphics/gl.h>
#include <GLFW/glfw3.h>


extern GLFWwindow *window;
extern unsigned int window_width, window_height;
extern int cursor_dx, cursor_dy;

void glfw_on_mouse(GLFWwindow *window, double dx, double dy);
void gldw_on_reshape(GLFWwindow *window, int w, int h);
void glfw_on_error(int error, const char *desc);
int init_glfw(void);
void free_glfw(void);


#endif // GRAPHICS_GLFW_H
