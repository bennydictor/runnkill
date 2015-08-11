#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
//#include <sys/types.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/freeglut.h>

#include <log.h>
#include <shader.h>
#include <vecmath.h>
#include <sphere_bo.h>

#ifndef M_PI
#  define M_PI		3.14159265358979323846
#endif

unsigned int prog;
unsigned int vbo, ibo;
mat4 mat_m, mat_v, mat_p;

unsigned int unif_v_mat_m, unif_v_mat_v, unif_v_mat_p;
unsigned int attr_v_coord, attr_v_normal;

vec3 pos;

unsigned int prev_time;
unsigned int fps;
unsigned int last_point;

float delta() {
    ++fps;
    unsigned int cur_time = glutGet(GLUT_ELAPSED_TIME);
    if (cur_time - last_point >= 1000) {
        printl(LOG_D, "fps: %d\n", fps);
        fps = 0;
        last_point = cur_time;
    }
    float d = (cur_time - prev_time) / 1000.0f;
    prev_time = cur_time;
    return d;
}

#define INIT_ATTR(ATTR) do { \
    attr_##ATTR = glGetAttribLocation(prog, #ATTR); \
    if (attr_##ATTR == -1U) { \
        printl(LOG_E, "Error while initializing resources: cannot bind attribute " #ATTR "\n"); \
        return -1; \
    } \
} while (0)
#define INIT_UNIF(UNIF) do { \
    unif_##UNIF = glGetUniformLocation(prog, #UNIF); \
    if (unif_##UNIF == -1U) { \
        printl(LOG_E, "Error while initializing resources: cannot bind uniform " #UNIF "\n"); \
        return -1; \
    } \
} while (0)

int init_resources(void) {
    mat_m = make_mat4();
    mat_v = make_mat4();
    mat_p = make_mat4();
    id_mat4(mat_m);
    id_mat4(mat_v);
    persp_mat(1.5, 1366.0 / 768.0, .0001, 1000, mat_p);
    pos = make_vec3(0, 0, 0);

    pos[0] = -5;
    pos[2] = -5;
    trans_mat(pos, mat_m);
    pos[2] = 0;

    unsigned int vs = create_shader("shaders/light_vertex.glsl", GL_VERTEX_SHADER);
    if (vs == -1U) {
        printl(LOG_E, "Error while initializing resources: cannot compile vertex shader.\n");
        return -1;
    }

    unsigned int fs = create_shader("shaders/light_fragment.glsl", GL_FRAGMENT_SHADER);
    if (fs == -1U) {
        printl(LOG_E, "Error while initializing resources: cannot compile fragment shader.\n");
        return -1;
    }

    prog = create_program(vs, fs);
    if (prog == -1U) {
        printl(LOG_E, "Error while initializing resources: cannot compile program.\n");
        return -1;
    }

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vbo_data), sphere_vbo_data, GL_STATIC_DRAW);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_ibo_data), sphere_ibo_data, GL_STATIC_DRAW);

    INIT_ATTR(v_coord);
    INIT_ATTR(v_normal);

    INIT_UNIF(v_mat_m);
    INIT_UNIF(v_mat_v);
    INIT_UNIF(v_mat_p);

    glUseProgram(prog);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    return 0;
}

void on_display(void) {
    glClearColor(0, 0, 0, 1);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUniformMatrix4fv(unif_v_mat_m, 1, GL_FALSE, mat_m);
    glUniformMatrix4fv(unif_v_mat_v, 1, GL_FALSE, mat_v);
    glUniformMatrix4fv(unif_v_mat_p, 1, GL_FALSE, mat_p);

    glEnableVertexAttribArray(attr_v_coord);
    glEnableVertexAttribArray(attr_v_normal);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(attr_v_coord, 3, GL_FLOAT, GL_FALSE, sizeof(vertex3d), (void *) offsetof(vertex3d, coord));
    glVertexAttribPointer(attr_v_normal, 3, GL_FLOAT, GL_FALSE, sizeof(vertex3d), (void *) offsetof(vertex3d, normal));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glDrawElements(GL_QUADS, sizeof(sphere_ibo_data) / sizeof(short int), GL_UNSIGNED_SHORT, sphere_ibo_data);

    glDisableVertexAttribArray(attr_v_coord);
    glDisableVertexAttribArray(attr_v_normal);

    glutSwapBuffers();
}

void on_idle() {
    pos[0] = delta();
    itrans_mat(pos, mat_m);
    glutPostRedisplay();
}

void free_resources(void) {
    glDeleteProgram(prog);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    free(mat_m);
    free(mat_v);
    free(mat_p);
    lclose();
}

int main(int argc, char** argv) {
    lopen("/dev/stderr");
    min_log_level = LOG_D;

    glutInit(&argc, argv);
    glutInitContextVersion(2, 0);
    glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1366, 768);
    glutCreateWindow("cube");

    GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK) {
        fprintf(stderr, "%s\n", glewGetErrorString(glew_status));
        return EXIT_FAILURE;
    }
    delta();
    if (init_resources() != -1) {
        glutDisplayFunc(on_display);
        glutIdleFunc(on_idle);
        glutMainLoop();
    }

    free_resources();
    return EXIT_SUCCESS;
}
