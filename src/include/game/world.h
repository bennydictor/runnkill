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

void world_callback(std::vector<draw_obj> &res, vec3f coord);
void man_update(int man_idx, char* pressed, vec3<float> curr_orientation);

extern "C" {
#endif

extern int world_max_height;
extern draw_obj world_map;
int init_world(void);
void world_update(float dt, char *evs, vec3f rot);
void free_world(void);

#ifdef __cplusplus
}
#endif

#endif // GAME_WORLD_H
