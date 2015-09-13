in vec2 f_texcoord;
out vec4 color;

uniform sampler2D f_texture;

void main(void) {
    color = texture2D(f_texture, f_texcoord);
    vec2 texcoord = f_texcoord;
    texcoord += vec2(1.0, 1.0);
    texcoord.x *= 1366.0 / 2.0;
    texcoord.y *= 768.0 / 2.0;
    float dx = (texcoord.x / 3) - floor(texcoord.x / 3);
    float dy = (texcoord.y / 3) - floor(texcoord.y / 3);
    if (dx < .3) {
        color.g *= .7;
        color.b *= .7;
    } else if (dx < .6) {
        color.r *= .7;
        color.b *= .7;
    } else {
        color.r *= .7;
        color.g *= .7;
    }
}
