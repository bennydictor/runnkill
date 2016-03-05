#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#define WORLD_EVENT_COUNT 20
#define WORLD_MOVE_FORWARD_EVENT 0
#define WORLD_MOVE_RIGHT_EVENT 1
#define WORLD_MOVE_BACKWARD_EVENT 2
#define WORLD_MOVE_LEFT_EVENT 3
#define WORLD_ATTACK_EVENT 4
#define WORLD_SYM_1 5
#define WORLD_SYM_2 6
#define WORLD_SYM_3 7
#define WORLD_SYM_4 8
#define WORLD_SYM_5 9
#define WORLD_SYM_6 10
#define WORLD_SYM_7 11
#define WORLD_SYM_8 12
#define WORLD_SYM_9 13
#define WORLD_SYM_0 14
#define WORLD_BLOCK_UL 15
#define WORLD_BLOCK_UR 16
#define WORLD_BLOCK_DL 17
#define WORLD_BLOCK_DR 18
#define WORLD_RUN_EVENT 19
#define LIGHT_COUNT 8
#define BETWEEN_SAVING 20

typedef struct {
    char type;
    float pos[3];
    float rad;
    float rot;
    int s;
    short int material;
} draw_obj;

#ifdef __cplusplus

#include <vector>
#include <math/vec3.h>
#include <game/skills/skill_type.h>
#include <game/items.h>
//#include <game/armour.h>
#include <game/animation.h>
#include <game/man.h>

extern std::vector<std::vector<abstract_skill_t *>> default_skills;
extern std::vector<std::vector<int>> skills_amounts;
extern std::vector<item_t> default_items;
//extern std::vector<armour> default_armours;
extern std::vector<animation> animations;
extern std::vector<int> fake_materials_idx;
extern vec3<float> sector_points_a[8];
extern vec3<float> sector_points_b[8];
extern vec3<float> sector_points_c[8];
extern std::vector<man*> persons;
extern std::vector<char> is_alive;
void man_update(int man_idx, char* pressed, vec3<float> curr_orientation);

extern "C" {
#endif

#include <math/vecmath.h>

#define DRAW_SPHERE 0
#define DRAW_SPHERE_SECTOR 1
#define DRAW_CIRCLE 2
#define MAX_DRAW_OBJ 256

typedef struct {
    vec3f pos;
    vec3f rot;
    vec3f ambient;
    vec3f diffuse;
    vec3f specular;
    float fov;
    float z_near;
    float z_far;
} light_t;

extern light_t gl_light[LIGHT_COUNT];
extern char gl_light_enable[LIGHT_COUNT];
extern draw_obj draw_objs[MAX_DRAW_OBJ];
extern int draw_obj_count;

extern int world_w, world_h;
extern int **world_field;
extern int world_max_height;
void world_callback(void);
void world_update(float dt);
void kill_person(int idx);
void save_player(int idx);
char load_player(char* name);
void *get_person_data_begin(int idx);
void *get_person_data_end(int idx);
char *get_person_text(int idx);
float get_person_business(int idx);
float get_person_max_business(int idx);
//extern draw_obj **world_map;
//void world_update(float dt, char *evs, vec3f rot, float* hp, float* mp);

#ifdef __cplusplus
}
#endif

#endif // GAME_WORLD_H
