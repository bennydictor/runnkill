 #define LIGHT_COUNT 8

varying vec3 f_coord;
varying vec3 f_normal;

uniform struct {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
} f_material;

uniform struct {
    vec3 coord;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} f_light[LIGHT_COUNT];

uniform int f_light_enable[LIGHT_COUNT];

uniform mat4 v_mat_m;
uniform mat4 v_mat_v;
uniform mat4 v_mat_p;

void main() {
    gl_FragColor.rgba = vec4(0, 0, 0, 1);
    vec3 ambient_fc = vec3(0, 0, 0);
    vec3 diffuse_fc = vec3(0, 0, 0);
    vec3 specular_fc = vec3(0, 0, 0);
    
    vec3 vertex_position = (v_mat_v * v_mat_m * vec4(f_coord, 1)).xyz;
    vec3 surface_normal = normalize((v_mat_v * v_mat_m * vec4(f_normal, 0)).xyz);
    for (int i = 0; i < LIGHT_COUNT; ++i) {
        if (f_light_enable[i] != 0) {
            ambient_fc += f_light[i].ambient;
            vec3 light_position = (v_mat_v * vec4(f_light[i].coord, 1)).xyz;
            vec3 light_direction = normalize(light_position - vertex_position);
            float diffuse_light_intensity = max(0, dot(surface_normal, light_direction));
            diffuse_fc += diffuse_light_intensity * f_light[i].diffuse;
            vec3 half_vector = normalize(-normalize(vertex_position) + light_direction);
            float specular_light_intensity = max(0, dot(surface_normal, half_vector));
            if (diffuse_light_intensity != 0) {
                specular_light_intensity = pow(specular_light_intensity, f_material.shininess);
                specular_fc += specular_light_intensity * f_light[i].specular;
            }
        }
    }
    gl_FragColor.rgb = f_material.ambient * ambient_fc +
                       f_material.diffuse * diffuse_fc +
                       f_material.specular * specular_fc;
}
