#ifndef VECMATH_H
#define VECMATH_H


#define INDEX3(x, y) (3 * x + y)
#define INDEX4(x, y) (4 * x + y)

#ifdef __cplusplus
extern "C" {
#endif

typedef float* vec3;
typedef float* vec4;

vec3 make_vec3_0();
vec4 make_vec4_0();
vec3 make_vec3(float x, float y, float z);
vec4 make_vec4(float x, float y, float z, float w);

typedef float* mat3;
typedef float* mat4;

mat3 make_mat3();
mat4 make_mat4();

vec3 copy_vec3(vec3 v);
vec4 copy_vec4(vec4 v);
mat3 copy_mat3(mat4 m);
mat4 copy_mat4(mat4 m);

void id_mat3(mat3 res);
void id_mat4(mat4 res);
void null_mat3(mat3 res);
void null_mat4(mat4 res);
void trans_mat(vec3 t, mat4 res);
void scale_mat(vec3 s, mat4 res);
void rot_x_mat(float angle, mat4 res);
void rot_y_mat(float angle, mat4 res);
void rot_z_mat(float angle, mat4 res);

void itrans_mat(vec3 t, mat4 res);
void iscale_mat(vec3 s, mat4 res);
void irot_x_mat(float angle, mat4 res);
void irot_y_mat(float angle, mat4 res);
void irot_z_mat(float angle, mat4 res);

void mul_mat3mv(mat3 a, vec3 b, vec3 res);
void mul_mat3mm(mat3 a, mat3 b, mat3 res);
void mul_mat4mv(mat4 a, vec4 b, vec4 res);
void mul_mat4mm(mat4 a, mat4 b, mat4 res);

void ortho_mat(float left, float right, float top, float bottom, float z_near, float z_far, mat4 res);
void persp_mat(float fov, float aspect, float z_near, float z_far, mat4 res);

#ifdef __cplusplus
}
#endif

#endif // VECMATH_H
