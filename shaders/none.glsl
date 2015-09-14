varying vec2 f_texcoord;

uniform sampler2D f_texture;

void main(void) {
    gl_FragColor = texture2D(f_texture, f_texcoord);
}
