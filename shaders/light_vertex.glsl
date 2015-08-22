uniform mat4 v_mat_m;
uniform mat4 v_mat_v;
uniform mat4 v_mat_p;
attribute vec3 v_coord;
attribute vec2 v_tex_coord;
attribute vec3 v_normal;
attribute vec3 v_color;
varying vec3 f_coord;
varying vec2 f_tex_coord;
varying vec3 f_normal;
varying vec3 f_color;

void main() {
    mat4 mvp = v_mat_p * v_mat_v * v_mat_m;
    gl_Position = mvp * vec4(v_coord, 1);
    f_coord = v_coord;
    f_normal = v_normal;
    f_tex_coord = v_tex_coord;
    f_color = v_color;
}
