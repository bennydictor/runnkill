uniform sampler2D f_texture;
varying vec2 f_texcoord;

void main(void) {
    gl_FragColor = texture2D(f_texture, f_texcoord);
}
