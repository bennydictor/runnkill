#ifndef VECMATH_H
#define VECMATH_H


#define INDEX3(x, y) (3 * x + y)
#define INDEX4(x, y) (4 * x + y)

#ifdef __cplusplus
extern "C" {
#endif

typedef float* vec3f;
typedef float* vec4f;

vec3f make_vec3_0();
vec4f make_vec4_0();
vec3f make_vec3(float x, float y, float z);
vec4f make_vec4(float x, float y, float z, float w);

typedef float* mat3f;
typedef float* mat4f;

mat3f make_mat3();
mat4f make_mat4();

vec3f copy_vec3(vec3f v);
vec4f copy_vec4(vec4f v);
mat3f copy_mat3(mat4f m);
mat4f copy_mat4(mat4f m);

void id_mat3(mat3f res);
void id_mat4(mat4f res);
void null_mat3(mat3f res);
void null_mat4(mat4f res);
void trans_mat(vec3f t, mat4f res);
void scale_mat(vec3f s, mat4f res);
void rot_x_mat(float angle, mat4f res);
void rot_y_mat(float angle, mat4f res);
void rot_z_mat(float angle, mat4f res);

void itrans_mat(vec3f t, mat4f res);
void iscale_mat(vec3f s, mat4f res);
void irot_x_mat(float angle, mat4f res);
void irot_y_mat(float angle, mat4f res);
void irot_z_mat(float angle, mat4f res);

void mul_mat3mv(mat3f a, vec3f b, vec3f res);
void mul_mat3mm(mat3f a, mat3f b, mat3f res);
void mul_mat4mv(mat4f a, vec4f b, vec4f res);
void mul_mat4mm(mat4f a, mat4f b, mat4f res);

void ortho_mat(float left, float right, float top, float bottom, float z_near, float z_far, mat4f res);
void persp_mat(float fov, float aspect, float z_near, float z_far, mat4f res);

#ifdef __cplusplus
}
#endif

#endif // VECMATH_H
