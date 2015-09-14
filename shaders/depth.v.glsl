attribute vec3 v_coord;

uniform mat4 v_mat_m;
uniform mat4 v_mat_v;
uniform mat4 v_mat_p;

void main() {
    mat4 mvp = v_mat_p * v_mat_v * v_mat_m;
    gl_Position = mvp * vec4(v_coord, 1);
}
