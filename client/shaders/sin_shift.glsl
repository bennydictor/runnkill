varying vec2 f_texcoord;

uniform sampler2D texture;

void main(void) {
    vec2 texcoord = f_texcoord;
    texcoord.x += sin(texcoord.y * 4*2*3.14159) / 100;
    gl_FragColor = texture2D(texture, texcoord);
}
