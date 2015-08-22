#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#ifdef __cplusplus
extern "C" {
#endif

void print_status_log(unsigned int log_level, unsigned int id);

unsigned int create_shader(const char *filename, GLenum type);

unsigned int create_program(GLuint vs, unsigned int fs);

#ifdef __cplusplus
}
#endif

#endif // SHADER_H
