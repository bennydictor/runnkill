#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/freeglut.h>

#include <graphics/shader.h>
#include <graphics/sphere_bo.h>
#include <util/log.h>
#include <math/vecmath.h>


unsigned int prog;
unsigned int vbo;
mat4 mat_m, mat_v, mat_p;

unsigned int attr_v_coord, attr_v_normal;

unsigned int unif_v_mat_m, unif_v_mat_v, unif_v_mat_p;

float angle;
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
    init_sphere();
    mat_m = make_mat4();
    mat_v = make_mat4();
    mat_p = make_mat4();
    id_mat4(mat_m);
    id_mat4(mat_v);
    id_mat4(mat_p);
    persp_mat(1, 1366.0 / 768.0, .0001, 1000, mat_p);
    pos = make_vec3(-5, 0, -5);
    trans_mat(pos, mat_v);
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

    glUniform3f(glGetUniformLocation(prog, "f_material.ambient"), 1, 1, 1);
    glUniform3f(glGetUniformLocation(prog, "f_material.diffuse"), 1, 1, 1);
    glUniform3f(glGetUniformLocation(prog, "f_material.specular"), 1, 1, 1);
    glUniform1f(glGetUniformLocation(prog, "f_material.shininess"), 128);

    glUniform3f(glGetUniformLocation(prog, "f_light[0].coord"), 0, 3, -5);
    glUniform3f(glGetUniformLocation(prog, "f_light[0].ambient"), .2, .2, .2);
    glUniform3f(glGetUniformLocation(prog, "f_light[0].diffuse"), .9, .9, .9);
    glUniform3f(glGetUniformLocation(prog, "f_light[0].specular"), 1, 1, 1);

    glUniform1i(glGetUniformLocation(prog, "f_light_enable[0]"), 1);
    glUniform1i(glGetUniformLocation(prog, "f_light_enable[1]"), 0);
    glUniform1i(glGetUniformLocation(prog, "f_light_enable[2]"), 0);
    glUniform1i(glGetUniformLocation(prog, "f_light_enable[3]"), 0);
    glUniform1i(glGetUniformLocation(prog, "f_light_enable[4]"), 0);
    glUniform1i(glGetUniformLocation(prog, "f_light_enable[5]"), 0);
    glUniform1i(glGetUniformLocation(prog, "f_light_enable[6]"), 0);
    glUniform1i(glGetUniformLocation(prog, "f_light_enable[7]"), 0);

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

    glDrawElements(GL_QUADS, sizeof(sphere_ibo_data) / sizeof(short int), GL_UNSIGNED_SHORT, sphere_ibo_data);

    glDisableVertexAttribArray(attr_v_coord);
    glDisableVertexAttribArray(attr_v_normal);

    glutSwapBuffers();
}

void on_idle() {
    pos[0] = angle = delta();
    irot_x_mat(angle, mat_m);
    itrans_mat(pos, mat_v);
    glutPostRedisplay();
}

void free_resources(void) {
    glDeleteProgram(prog);
    glDeleteBuffers(1, &vbo);
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
    glutCreateWindow("sphere");

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
