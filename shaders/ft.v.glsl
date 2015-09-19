attribute vec2 v_coord;
attribute vec2 v_texcoord;

varying vec2 f_coord;
varying vec2 f_texcoord;

uniform mat4 mat_p;

void main(void) {
    gl_Position = mat_p * vec4(v_coord.xy, 0, 1);
    f_coord = v_coord;
    f_texcoord = v_texcoord;
}
