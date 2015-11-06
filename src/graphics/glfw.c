#include <graphics/glfw.h>
#include <graphics/gl.h>
#include <math/vecmath.h>
#include <util/log.h>


#define unused(X) ((void) (X))

GLFWwindow *window;
unsigned int window_width, window_height;
int cursor_dx, cursor_dy;

void glfw_on_mouse(GLFWwindow *win, double dx, double dy) {
    int x = dx;
    int y = dy;
    cursor_dx = x - (window_width / 2);
    cursor_dy = y - (window_height / 2);
    glfwSetCursorPos(win, window_width / 2, window_height / 2);
}

void glfw_on_reshape(GLFWwindow *win, int w, int h) {
    unused(win);
    window_width = w;
    window_height = h;
    gl_reshape();
}

void glfw_on_error(int error, const char *desc) {
    printl(LOG_W, "GLFW error (%d), %s\n", error, desc);
}

int init_glfw(void) {
    if(!glfwInit()) {
        printl(LOG_W, "Failed it init GLFW\n");
        return 1;
    }

    glfwSetErrorCallback(glfw_on_error);
    window = glfwCreateWindow(window_width = 1366, window_height = 768, "shpere", NULL, NULL);
    if (!window) {
        printl(LOG_W, "Failed to create window\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, glfw_on_reshape);
    glfwSetCursorPosCallback(window, glfw_on_mouse);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return 0;
}

void free_glfw(void) {
    glfwDestroyWindow(window);
    glfwTerminate();
}
