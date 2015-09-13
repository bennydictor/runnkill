in vec2 f_texcoord;
out vec4 color;

uniform sampler2D f_texture;

void main(void) {
    color = texture2D(f_texture, f_texcoord);
}
