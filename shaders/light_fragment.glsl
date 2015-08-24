varying vec3 f_coord;
<<<<<<< HEAD
varying vec2 f_tex_coord;
varying vec3 f_normal;
varying vec3 f_color;

=======
varying vec3 f_normal;

uniform vec3 f_color;
uniform vec3 f_light;
>>>>>>> benny
uniform mat4 v_mat_m;
uniform mat4 v_mat_v;
uniform mat4 v_mat_p;
uniform sampler2D f_tex;

void main() {
<<<<<<< HEAD
    gl_FragColor = texture2D(f_tex, f_tex_coord);
    gl_FragColor.xyz *= f_color.xyz;
    vec4 light_color = vec4(0.2, 0.2, 0.2, 0.2);

    vec3 vertex_position = (v_mat_v * v_mat_m * vec4(f_coord, 1)).xyz;
    vec3 surface_normal = normalize((gl_NormalMatrix * f_normal).xyz);
    vec3 light_direction = normalize(vec4(10, 5, 2, 1).xyz - vertex_position);
=======
    gl_FragColor.rgba = vec4(f_color, 1);
    vec4 light_color = vec4(0.2, 0.2, 0.2, 0.2);

    vec3 light_position = (vec4(f_light, 1)).xyz;
    vec3 vertex_position = (v_mat_v * v_mat_m * vec4(f_coord, 1)).xyz;
    vec3 surface_normal = normalize((v_mat_m * vec4(f_normal, 1)).xyz);
    vec3 light_direction = normalize(light_position - vertex_position);
>>>>>>> benny
    float diffuse_light_intensity = max(0, dot(surface_normal, light_direction));
    light_color += diffuse_light_intensity;
    gl_FragColor.rgb *= light_color.rgb;
    vec3 half_vector = normalize(-normalize(vertex_position) + light_direction);
    float specular = max(0, dot(surface_normal, half_vector));
    if (diffuse_light_intensity != 0) {
        specular = pow(specular, 256);
        gl_FragColor.rgb += specular;
    }
}
