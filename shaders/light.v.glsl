attribute vec3 v_coord;
attribute vec3 v_normal;

varying vec3 f_coord;
varying vec3 f_normal;

uniform mat4 mat_m;
uniform mat4 mat_v;
uniform mat4 mat_p;

void main() {
    gl_Position = mat_p * mat_v * mat_m * vec4(v_coord, 1);
    f_coord = v_coord;
    f_normal = v_normal;
}
