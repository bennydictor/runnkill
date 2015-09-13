in vec2 f_coord;
in vec2 f_texcoord;
out vec4 color;

uniform vec2 CENTER = vec2(0, 0);
uniform float RADIUS = 0.8;
uniform float ANGLE = 3;

uniform sampler2D f_texture;

void main(void) {
    vec2 coord = f_coord;
    coord -= CENTER;
    float dist = length(coord);
    if (dist < RADIUS) {
        float ratio = (RADIUS - dist) / RADIUS;
        float angle = ratio * ratio * ANGLE;
        float s = sin(angle);
        float c = cos(angle);
        coord = vec2(dot(coord, vec2(c, -s)), dot(coord, vec2(s, c)));
    }
    coord += CENTER;
    coord += vec2(1, 1);
    coord /= 2;
    color = texture2D(f_texture, coord);
}
