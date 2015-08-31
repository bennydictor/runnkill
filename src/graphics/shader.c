#include <graphics/shader.h>

#include <stdlib.h>

#include <GL/glew.h>

#include <util/log.h>
#include <util/file.h>


void print_status_log(GLuint id) {
    GLint log_len = 0;
    if (glIsShader(id)) {
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_len);
    } else if (glIsProgram(id)) {
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &log_len);
    } else {
        printl(LOG_W, "Error while printing log: %d is not a shader or a program.\n");
        return;
    }
    char *log = malloc(log_len * sizeof(char));
    if (glIsShader(id)) {
        glGetShaderInfoLog(id, log_len, NULL, log);
    } else {
        glGetProgramInfoLog(id, log_len, NULL, log);
    }
    printl(LOG_I, log);
    free(log);
}

GLuint create_shader(const char *filename, GLenum type) {
    GLchar *source = read_file(filename);
    if (source == NULL) {
        printl(LOG_W, "Error while compiling shader %s: source cannot be read.\n", filename);
        return -1;
    }
    GLuint shader = glCreateShader(type);
    const GLchar *sources[2] = {
#ifdef GL_ES_VERSION_2_0
        "#version 100\n"
        "#define GLES2\n",
#else
        "#version 120\n",
#endif
        source
    };
    glShaderSource(shader, 2, sources, NULL);
    free(source);
    glCompileShader(shader);
    GLint compile_status = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
    if (compile_status == GL_FALSE) {
        printl(LOG_W, "Error while compiling shader %s:\n", filename);
        print_status_log(shader);
        glDeleteShader(shader);
        return -1;
    }
    return shader;
}

GLuint create_program(GLuint vs, GLuint fs) {
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);
    GLint link_status = GL_FALSE;
    glGetProgramiv(prog, GL_LINK_STATUS, &link_status);
    if (link_status == GL_FALSE) {
        printl(LOG_W, "Error while linking program:\n");
        print_status_log(prog);
        glDeleteProgram(prog);
        return -1;
    }
    return prog;
}
