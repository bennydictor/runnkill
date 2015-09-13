 #define LIGHT_COUNT 8
in vec3 v_coord;
in vec3 v_normal;

out vec3 f_coord;
out vec3 f_normal;

uniform mat4 v_mat_m;
uniform mat4 v_mat_v;
uniform mat4 v_mat_p;

uniform int f_light_enable[LIGHT_COUNT];

void main() {
    gl_Position = v_mat_p * v_mat_v * v_mat_m * vec4(v_coord, 1);
    f_coord = v_coord;
    f_normal = v_normal;
}
