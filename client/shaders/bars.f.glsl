varying vec2 f_coord;

uniform mat4 mat_p;
uniform mat4 mat_m;
uniform vec3 color;

void main(void) {
    gl_FragColor = vec4(color.rgb, 1);
}
