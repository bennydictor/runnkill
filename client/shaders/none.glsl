varying vec2 f_texcoord;

uniform sampler2D texture;

void main(void) {
    vec2 texcoord = f_texcoord;
    gl_FragColor = texture2D(texture, texcoord);
}
