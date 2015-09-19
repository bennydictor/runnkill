#ifndef MATH_VECMATH_H
#define MATH_VECMATH_H

#define INDEX3(x, y) (3 * x + y)
#define INDEX4(x, y) (4 * x + y)

#include <stdlib.h>


typedef float* vec3f;

vec3f make_vec3_0();
vec3f make_vec3(float x, float y, float z);
vec3f set_vec3f(float x, float y, float z, vec3f v);
vec3f add_vec3f(float x, float y, float z, vec3f v);
vec3f sub_vec3f(float x, float y, float z, vec3f v);
vec3f set_vec3fv(vec3f dst, vec3f src);
vec3f add_vec3fv(vec3f dst, vec3f src);
vec3f sub_vec3fv(vec3f dst, vec3f src);
vec3f mul_vec3f(float x, vec3f v);

typedef float* mat4f;

mat4f make_mat4();

vec3f copy_vec3(vec3f v);
mat4f copy_mat4(mat4f m);

mat4f id_mat4(mat4f res);
mat4f null_mat4(mat4f res);

mat4f trans_mat(vec3f t, mat4f res);
mat4f scale_mat(vec3f s, mat4f res);
mat4f rot_x_mat(float angle, mat4f res);
mat4f rot_y_mat(float angle, mat4f res);
mat4f rot_z_mat(float angle, mat4f res);

mat4f itrans_mat(vec3f t, mat4f res);
mat4f iscale_mat(vec3f s, mat4f res);
mat4f irot_x_mat(float angle, mat4f res);
mat4f irot_y_mat(float angle, mat4f res);
mat4f irot_z_mat(float angle, mat4f res);

mat4f mul_mat4mm(mat4f a, mat4f b, mat4f res);

mat4f ortho_mat(float left, float right, float top, float bottom, float z_near, float z_far, mat4f res);
mat4f persp_mat(float fov, float aspect, float z_near, float z_far, mat4f res);

#endif // MATH_VECMATH_H
