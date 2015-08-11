#include <vecmath.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


vec3 make_vec3_0() {
    vec3 res = malloc(3 * sizeof(float));
    return res;
}

vec4 make_vec4_0() {
    vec4 res = malloc(4 * sizeof(float));
    return res;
}

vec3 make_vec3(float x, float y, float z) {
    vec3 res = make_vec3_0();
    res[0] = x;
    res[1] = y;
    res[2] = z;
    return res;
}

vec4 make_vec4(float x, float y, float z, float w) {
    vec4 res = make_vec4_0();
    res[0] = x;
    res[1] = y;
    res[2] = z;
    res[3] = w;
    return res;
}

mat3 make_mat3() {
    mat3 res = malloc(9 * sizeof(float));
    return res;
}

mat4 make_mat4() {
    mat4 res = malloc(16 * sizeof(float));
    return res;
}

vec3 copy_vec3(vec3 v) {
    vec3 ret = make_vec3_0();
    memcpy(ret, v, 3 * sizeof(float));
    return ret;
}

vec4 copy_vec4(vec4 v) {
    vec4 ret = make_vec4_0();
    memcpy(ret, v, 4 * sizeof(float));
    return ret;
}

mat3 copy_mat3(mat3 m) {
    mat3 ret = make_mat3();
    memcpy(ret, m, 9 * sizeof(float));
    return ret;
}

mat4 copy_mat4(mat4 m) {
    mat4 ret = make_mat4();
    memcpy(ret, m, 16 * sizeof(float));
    return ret;
}

void null_mat3(mat3 res) {
    for (int i = 0; i < 9; i++) {
        res[i] = 0;
    }
}

void null_mat4(mat4 res) {
    for (int i = 0; i < 16; i++) {
        res[i] = 0;
    }
}

void id_mat3(mat3 res) {
    null_mat3(res);
    res[INDEX3(0, 0)] = res[INDEX3(1, 1)] = res[INDEX3(2, 2)] = 1;
}

void id_mat4(mat4 res) {
    null_mat4(res);
    res[INDEX4(0, 0)] = res[INDEX4(1, 1)] = res[INDEX4(2, 2)] = res[INDEX4(3, 3)] = 1;
}

void trans_mat(vec3 t, mat4 res) {
    id_mat4(res);
    res[INDEX4(3, 0)] = t[0];
    res[INDEX4(3, 1)] = t[1];
    res[INDEX4(3, 2)] = t[2];
}

void scale_mat(vec3 s, mat4 res) {
    id_mat4(res);
    res[INDEX4(0, 0)] = s[0];
    res[INDEX4(1, 1)] = s[1];
    res[INDEX4(2, 2)] = s[2];
}

void rot_x_mat(float angle, mat4 res) {
    id_mat4(res);
    res[INDEX4(1, 1)] = +cosf(angle);
    res[INDEX4(2, 1)] = -sinf(angle);
    res[INDEX4(1, 2)] = +sinf(angle);
    res[INDEX4(2, 2)] = +cosf(angle);
}

void rot_y_mat(float angle, mat4 res) {
    id_mat4(res);
    res[INDEX4(0, 0)] = +cosf(angle);
    res[INDEX4(2, 0)] = +sinf(angle);
    res[INDEX4(0, 2)] = -sinf(angle);
    res[INDEX4(2, 2)] = +cosf(angle);
}

void rot_z_mat(float angle, mat4 res) {
    id_mat4(res);
    res[INDEX4(0, 0)] = +cosf(angle);
    res[INDEX4(1, 0)] = -sinf(angle);
    res[INDEX4(0, 1)] = +sinf(angle);
    res[INDEX4(1, 1)] = +cosf(angle);
}


void itrans_mat(vec3 t, mat4 res) {
    mat4 cres = copy_mat4(res);
    mat4 tres = make_mat4();
    trans_mat(t, tres);
    mul_mat4mm(cres, tres, res);
    free(cres);
    free(tres);
}

void iscale_mat(vec3 s, mat4 res) {
    mat4 cres = copy_mat4(res);
    mat4 tres = make_mat4();
    scale_mat(s, tres);
    mul_mat4mm(cres, tres, res);
    free(cres);
    free(tres);
}

void irot_x_mat(float angle, mat4 res) {
    mat4 cres = copy_mat4(res);
    mat4 tres = make_mat4();
    rot_x_mat(angle, tres);
    mul_mat4mm(cres, tres, res);
    free(cres);
    free(tres);
}

void irot_y_mat(float angle, mat4 res) {
    mat4 cres = copy_mat4(res);
    mat4 tres = make_mat4();
    rot_y_mat(angle, tres);
    mul_mat4mm(cres, tres, res);
    free(cres);
    free(tres);
}

void irot_z_mat(float angle, mat4 res) {
    mat4 cres = copy_mat4(res);
    mat4 tres = make_mat4();
    rot_y_mat(angle, tres);
    mul_mat4mm(cres, tres, res);
    free(cres);
    free(tres);
}

void mul_mat3mv(mat3 a, vec3 b, vec3 res) {
    for (int i = 0; i < 3; i++) {
        res[i] = 0;
        for (int j = 0; j < 3; j++) {
            res[i] += a[INDEX3(i, j)] * b[j];
        }
    }
}

void mul_mat3mm(mat3 a, mat3 b, mat3 res) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            res[INDEX3(i, j)] = 0;
            for (int k = 0; k < 3; k++) {
                res[INDEX3(i, j)] += a[INDEX3(i, k)] * b[INDEX3(k, j)];
            }
        }
    }
}

void mul_mat4mv(mat4 a, vec4 b, vec4 res) {
    for (int i = 0; i < 4; i++) {
        res[i] = 0;
        for (int j = 0; j < 4; j++) {
            res[i] += a[INDEX4(i, j)] * b[j];
        }
    }
}

void mul_mat4mm(mat4 a, mat4 b, mat4 res) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res[INDEX4(i, j)] = 0;
            for (int k = 0; k < 4; k++) {
                res[INDEX4(i, j)] += a[INDEX4(i, k)] * b[INDEX4(k, j)];
            }
        }
    }
}

void ortho_mat(float left, float right, float bottom, float top, float z_near, float z_far, mat4 res) {
    id_mat4(res);
    res[INDEX4(0, 0)] = 2 / (right - left);
    res[INDEX4(1, 1)] = 2 / (top - bottom);
    res[INDEX4(2, 2)] = -2 / (z_far - z_near);
    res[INDEX4(3, 0)] = -(right + left) / (right - left);
    res[INDEX4(3, 1)] = -(top + bottom) / (top - bottom);
    res[INDEX4(3, 2)] = -(z_far + z_near) / (z_far - z_near);
}

void persp_mat(float fov, float aspect, float z_near, float z_far, mat4 res) {
    null_mat4(res);
    float f = tan(fov / 2);
    res[INDEX4(0, 0)] = 1 / (aspect * f);
    res[INDEX4(1, 1)] = 1 / f;
    res[INDEX4(2, 2)] = -(z_far + z_near) / (z_far - z_near);
    res[INDEX4(2, 3)] = -1;
    res[INDEX4(3, 2)] = -(2 * z_far * z_near) / (z_far - z_near);
}
