attribute vec2 v_coord;

varying vec2 f_coord;

uniform mat4 mat_p;
uniform mat4 mat_m;
uniform vec3 color;

void main(void) {
    gl_Position = mat_p * mat_m * vec4(v_coord.xy, 1, 1);
    f_coord = v_coord;
}
