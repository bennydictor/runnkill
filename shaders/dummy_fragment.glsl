varying vec3 f_color;
varying vec2 f_tex_coord;

uniform sampler2D f_tex;

void main(void) {
    gl_FragColor = texture2D(f_tex, f_tex_coord);
    gl_FragColor.xyz *= f_color.xyz;
}
