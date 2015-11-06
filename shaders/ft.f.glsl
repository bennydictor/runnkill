varying vec2 f_coord;
varying vec2 f_texcoord;

uniform mat4 mat_p;
uniform sampler2D texture;
uniform sampler2D colors;
uniform vec3 color;

mat4 bias = mat4(
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 0.5, 0.0,
    0.5, 0.5, 0.5, 1.0
);

vec3 rgb_to_hsl(vec3 color) {
    float r = color.r;
    float g = color.g;
    float b = color.b;
    float mx = max(max(r, g), b);
    float mn = min(min(r, g), b);
    float h = 0, s = 0, l = (mx + mn) / 2;
    if (mx != mn) {
        float d = mx - mn;
        s = l > 0.5 ? (d / (2 - mx - mn)) : (d / (mx + mn));
        if (mx == color.r) {
            h = (g - b) / d + (g < b ? 6 : 0);
        } else if (mx == color.g) {
            h = (b - r) / d + 2;
        } else {
            h = (r - g) / d + 4;
        }
        h /= 6;
    }
    return vec3(h, s, l);
}

float hue_to_rgb(float p, float q, float t) {
    if (t < 0) t += 1;
    if (t > 1) t -= 1;
    if (t < 1.0 / 6) return p + (q - p) * 6 * t;
    if (t < 1.0 / 2) return q;
    if (t < 2.0 / 3) return p + (q - p) * (2.0 / 3 - t) * 6;
    return p;
}

vec3 hsl_to_rgb(vec3 color) {
    float h = color.r;
    float s = color.g;
    float l = color.b;
    float r, g, b;
    if (s == 0) {
        r = g = b = l;
    } else {
        float q = l < 0.5 ? (l * (1 + s)) : (l + s - l * s);
        float p = 2 * l - q;
        r = hue_to_rgb(p, q, h + 1.0 / 3);
        g = hue_to_rgb(p, q, h);
        b = hue_to_rgb(p, q, h - 1.0 / 3);
    }
    return vec3(r, g, b);
}

vec3 opposite_color(vec3 color) {
    vec3 op = rgb_to_hsl(color);
    if (op.r < 0.5) {
        op.r += 0.5;
    }
    if (op.g  < 0.5) {
        op.g += 0.5;
    }
    op.b = 1 - op.b;
    return hsl_to_rgb(op);
}

void main(void) {
    vec3 minus = opposite_color(texture2D(colors, (bias * mat_p * vec4(f_coord, 0, 1)).xy).rgb);
    gl_FragColor = vec4(minus, texture2D(texture, f_texcoord).r);
}
