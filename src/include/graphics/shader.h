#ifndef GRAPHICS_SHADER_H
#define GRAPHICS_SHADER_H

#include <GL/glew.h>

void print_status_log(unsigned int id);
unsigned int create_shader(const char *filename, GLenum type);
unsigned int create_program(GLuint vs, unsigned int fs);


#endif // GRAPHICS_SHADER_H
