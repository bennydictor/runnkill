uniform vec3 f_color;

void main(void) {
    gl_FragColor.rgba = vec4(f_color, 1);
}
