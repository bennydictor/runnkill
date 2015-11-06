attribute vec3 v_coord;

uniform mat4 mat_m;
uniform mat4 mat_v;
uniform mat4 mat_p;

void main() {
    mat4 mvp = mat_p * mat_v * mat_m;
    gl_Position = mvp * vec4(v_coord, 1);
}
