#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include <graphics/draw_obj.h>

#define WORLD_EVENT_COUNT 5
#define WORLD_MOVE_FORWARD_EVENT 0
#define WORLD_MOVE_RIGHT_EVENT 1
#define WORLD_MOVE_BACKWARD_EVENT 2
#define WORLD_MOVE_LEFT_EVENT 3
#define WORLD_ATTACK_EVENT 4

#ifdef __cplusplus

#include <vector>
#include <math/vec3.h>
#include <game/skill_type.h>
#include <game/items.h>
#include <game/man.h>

extern int w, h, chunk;
extern std::vector<std::vector<skill_t > > default_skills;
extern std::vector<item_t> default_items;
extern vec3<float> sector_points_a[8];
extern vec3<float> sector_points_b[8];
extern vec3<float> sector_points_c[8];
extern int **F;
extern std::vector<man*> persons;
extern std::vector<bool> is_alive;
void world_callback(std::vector<draw_obj> &res, vec3f coord);
void man_update(int man_idx, char* pressed, vec3<float> curr_orientation);

extern "C" {
#endif

extern int world_max_height;
extern draw_obj **world_map;
void world_update(float dt, char *evs, vec3f rot, float* hp, float* mp);

#ifdef __cplusplus
}
#endif

#endif // GAME_WORLD_H
