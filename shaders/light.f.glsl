varying vec3 f_coord;
varying vec3 f_normal;

uniform struct {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
} material;

uniform struct {
    vec3 coord;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    mat4 mat_v;
    mat4 mat_p;
    float z_near;
    float z_far;
    sampler2D map;
} light[LIGHT_COUNT];

uniform int light_enable[LIGHT_COUNT];

uniform mat4 mat_m;
uniform mat4 mat_v;
uniform mat4 mat_p;

uniform float z_near;
uniform float z_far;

mat4 bias = mat4(
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 0.5, 0.0,
    0.5, 0.5, 0.5, 1.0
);

float lin_depth(float z_near, float z_far, float z) {
    return (2.0 * z_near) / (z_near + z_near - z * (z_far - z_near));
}

float shadow_radius = 1.0 / 100.0;
int samples_count = 2;

bool in_shadow(float true_depth, int i, vec2 coord) {
    return true_depth - lin_depth(light[i].z_near, light[i].z_far, texture2D(light[i].map, coord).r) > 1e10;
}

float pcf(vec4 light_coord, int i) {
    float true_depth = lin_depth(z_near, z_far, light_coord.z);
    bool smp[4];
    smp[0] = in_shadow(true_depth, i, light_coord.xy + vec2(-shadow_radius, -shadow_radius));
    smp[1] = in_shadow(true_depth, i, light_coord.xy + vec2(-shadow_radius, +shadow_radius));
    smp[2] = in_shadow(true_depth, i, light_coord.xy + vec2(+shadow_radius, +shadow_radius));
    smp[3] = in_shadow(true_depth, i, light_coord.xy + vec2(+shadow_radius, -shadow_radius));
    if (smp[0] && smp[1] && smp[2] && smp[3]) {
        return 0.0;
    }
    if (!smp[0] && !smp[1] && !smp[2] && !smp[3]) {
        return 1.0;
    }
    float ret = 0;
    int total_samples = 0;
    for (int x = -samples_count / 2; x <= samples_count / 2; ++x) {
        for (int y = -samples_count / 2; y <= samples_count / 2; ++y) {
            ++total_samples;
            vec2 delta = vec2(shadow_radius * x / samples_count / 2, shadow_radius * y / samples_count / 2);
            if (!in_shadow(true_depth, i, light_coord.xy + delta)) {
                ret += 1.0;
            }
        }
    }
    return (ret / total_samples) * (ret / total_samples);
}

void main() {
    gl_FragColor.rgba = vec4(0, 0, 0, 1);
    vec3 ambient_fc = vec3(0, 0, 0);
    vec3 diffuse_fc = vec3(0, 0, 0);
    vec3 specular_fc = vec3(0, 0, 0);
    
    vec3 vertex_position = (mat_v * mat_m * vec4(f_coord, 1)).xyz;
    vec3 surface_normal = normalize((mat_v * mat_m * vec4(f_normal, 0)).xyz);

    for (int i = 0; i < LIGHT_COUNT; ++i) {
        if (light_enable[i] != 0) {
            vec4 light_coord = bias * light[i].mat_p * light[i].mat_v * mat_m * vec4(f_coord, 1);
            light_coord /= light_coord.w;
            ambient_fc += light[i].ambient;
            float shadow = pcf(light_coord, i);
            vec3 light_position = (mat_v * vec4(light[i].coord, 1)).xyz;
            vec3 light_direction = normalize(light_position - vertex_position);
            float diffuse_light_intensity = max(0, dot(surface_normal, light_direction));
            diffuse_fc += diffuse_light_intensity * light[i].diffuse * shadow;
            vec3 half_vector = normalize(-normalize(vertex_position) + light_direction);
            float specular_light_intensity = max(0, dot(surface_normal, half_vector));
            if (diffuse_light_intensity > 1e-9) {
                specular_light_intensity = pow(specular_light_intensity, material.shininess);
                specular_fc += specular_light_intensity * light[i].specular * shadow;
            }
        }
    }
    gl_FragColor.rgb = material.ambient * ambient_fc +
                       material.diffuse * diffuse_fc +
                       material.specular * specular_fc;
}
