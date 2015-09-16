#include <math/vecmath.h>

#include <stdlib.h>
#include <math.h>
#include <string.h>


vec3f make_vec3_0() {
    vec3f res = malloc(3 * sizeof(float));
    return res;
}

vec3f set_vec3f(float x, float y, float z, vec3f v) {
    v[0] = x;
    v[1] = y;
    v[2] = z;
    return v;
}

vec3f add_vec3f(float x, float y, float z, vec3f v) {
    v[0] += x;
    v[1] += y;
    v[2] += z;
    return v;
}

vec3f sub_vec3f(float x, float y, float z, vec3f v) {
    v[0] -= x;
    v[1] -= y;
    v[2] -= z;
    return v;
}

vec3f set_vec3fv(vec3f dst, vec3f src) {
    return memcpy(dst, src, 3 * sizeof(float));
}

vec3f add_vec3fv(vec3f dst, vec3f src) {
    dst[0] += src[0];
    dst[1] += src[0];
    dst[2] += src[0];
    return dst;
}

vec3f sub_vec3fv(vec3f dst, vec3f src) {
    dst[0] -= src[0];
    dst[1] -= src[0];
    dst[2] -= src[0];
    return dst;
}

vec3f mul_vec3f(float x, vec3f v) {
    v[0] *= x;
    v[1] *= x;
    v[2] *= x;
    return v;
}

vec3f make_vec3(float x, float y, float z) {
    vec3f res = make_vec3_0();
    res[0] = x;
    res[1] = y;
    res[2] = z;
    return res;
}

mat4f make_mat4() {
    mat4f res = malloc(16 * sizeof(float));
    return res;
}

vec3f copy_vec3(vec3f v) {
    vec3f ret = make_vec3_0();
    memcpy(ret, v, 3 * sizeof(float));
    return ret;
}

mat4f copy_mat4(mat4f m) {
    mat4f ret = make_mat4();
    memcpy(ret, m, 16 * sizeof(float));
    return ret;
}

mat4f null_mat4(mat4f res) {
    for (int i = 0; i < 16; i++) {
        res[i] = 0;
    }
    return res;
}

mat4f id_mat4(mat4f res) {
    null_mat4(res);
    res[INDEX4(0, 0)] = res[INDEX4(1, 1)] = res[INDEX4(2, 2)] = res[INDEX4(3, 3)] = 1;
    return res;
}

mat4f trans_mat(vec3f t, mat4f res) {
    id_mat4(res);
    res[INDEX4(3, 0)] = t[0];
    res[INDEX4(3, 1)] = t[1];
    res[INDEX4(3, 2)] = t[2];
    return res;
}

mat4f scale_mat(vec3f s, mat4f res) {
    id_mat4(res);
    res[INDEX4(0, 0)] = s[0];
    res[INDEX4(1, 1)] = s[1];
    res[INDEX4(2, 2)] = s[2];
    return res;
}

mat4f rot_x_mat(float angle, mat4f res) {
    id_mat4(res);
    res[INDEX4(1, 1)] = +cosf(angle);
    res[INDEX4(2, 1)] = -sinf(angle);
    res[INDEX4(1, 2)] = +sinf(angle);
    res[INDEX4(2, 2)] = +cosf(angle);
    return res;
}

mat4f rot_y_mat(float angle, mat4f res) {
    id_mat4(res);
    res[INDEX4(0, 0)] = +cosf(angle);
    res[INDEX4(2, 0)] = +sinf(angle);
    res[INDEX4(0, 2)] = -sinf(angle);
    res[INDEX4(2, 2)] = +cosf(angle);
    return res;
}

mat4f rot_z_mat(float angle, mat4f res) {
    id_mat4(res);
    res[INDEX4(0, 0)] = +cosf(angle);
    res[INDEX4(1, 0)] = -sinf(angle);
    res[INDEX4(0, 1)] = +sinf(angle);
    res[INDEX4(1, 1)] = +cosf(angle);
    return res;
}

mat4f itrans_mat(vec3f t, mat4f res) {
    mat4f cres = copy_mat4(res);
    mat4f tres = make_mat4();
    trans_mat(t, tres);
    mul_mat4mm(cres, tres, res);
    free(cres);
    free(tres);
    return res;
}

mat4f iscale_mat(vec3f s, mat4f res) {
    mat4f cres = copy_mat4(res);
    mat4f tres = make_mat4();
    scale_mat(s, tres);
    mul_mat4mm(cres, tres, res);
    free(cres);
    free(tres);
    return res;
}

mat4f irot_x_mat(float angle, mat4f res) {
    mat4f cres = copy_mat4(res);
    mat4f tres = make_mat4();
    rot_x_mat(angle, tres);
    mul_mat4mm(cres, tres, res);
    free(cres);
    free(tres);
    return res;
}

mat4f irot_y_mat(float angle, mat4f res) {
    mat4f cres = copy_mat4(res);
    mat4f tres = make_mat4();
    rot_y_mat(angle, tres);
    mul_mat4mm(cres, tres, res);
    free(cres);
    free(tres);
    return res;
}

mat4f irot_z_mat(float angle, mat4f res) {
    mat4f cres = copy_mat4(res);
    mat4f tres = make_mat4();
    rot_y_mat(angle, tres);
    mul_mat4mm(cres, tres, res);
    free(cres);
    free(tres);
    return res;
}

mat4f mul_mat4mm(mat4f a, mat4f b, mat4f res) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res[INDEX4(i, j)] = 0;
            for (int k = 0; k < 4; k++) {
                res[INDEX4(i, j)] += a[INDEX4(i, k)] * b[INDEX4(k, j)];
            }
        }
    }
    return res;
}


mat4f ortho_mat(float left, float right, float bottom, float top, float z_near, float z_far, mat4f res) {
    id_mat4(res);
    res[INDEX4(0, 0)] = 2 / (right - left);
    res[INDEX4(1, 1)] = 2 / (top - bottom);
    res[INDEX4(2, 2)] = -2 / (z_far - z_near);
    res[INDEX4(0, 3)] = -(right + left) / (right - left);
    res[INDEX4(1, 3)] = -(top + bottom) / (top - bottom);
    res[INDEX4(2, 3)] = -(z_far + z_near) / (z_far - z_near);
    return res;
}

mat4f persp_mat(float fov, float aspect, float z_near, float z_far, mat4f res) {
    null_mat4(res);
    float f = tan(fov / 2);
    res[INDEX4(0, 0)] = 1 / (aspect * f);
    res[INDEX4(1, 1)] = 1 / f;
    res[INDEX4(2, 2)] = -(z_far + z_near) / (z_far - z_near);
    res[INDEX4(2, 3)] = -1;
    res[INDEX4(3, 2)] = -(2 * z_far * z_near) / (z_far - z_near);
    return res;
}
