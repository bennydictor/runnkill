varying vec2 f_texcoord;

uniform sampler2D texture;

void main(void) {
    gl_FragColor = texture2D(texture, f_texcoord);
    vec2 texcoord = f_texcoord;
    texcoord += vec2(1.0, 1.0);
    texcoord.x *= 1366.0 / 2.0;
    texcoord.y *= 768.0 / 2.0;
    float dx = (texcoord.x / 3) - floor(texcoord.x / 3);
    float dy = (texcoord.y / 3) - floor(texcoord.y / 3);
    if (dx < .3) {
        gl_FragColor.g *= .7;
        gl_FragColor.b *= .7;
    } else if (dx < .6) {
        gl_FragColor.r *= .7;
        gl_FragColor.b *= .7;
    } else {
        gl_FragColor.r *= .7;
        gl_FragColor.g *= .7;
    }
}
