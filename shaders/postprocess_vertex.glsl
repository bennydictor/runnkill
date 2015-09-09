attribute vec3 v_coord;
varying vec2 f_coord;
varying vec2 f_texcoord;


void main() {
    gl_Position = vec4(v_coord, 1.0);
    f_coord = v_coord.xy;
    f_texcoord = (v_coord.xy + vec2(1.0, 1.0)) / 2.0;
}
