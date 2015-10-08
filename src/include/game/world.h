#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include <graphics/draw_obj.h>

#ifdef __cplusplus

#include <vector>

#define __W 0
#define __D 1
#define __S 2
#define __A 3
#define SPACE 4
void world_draw_objs(std::vector<draw_obj> &res);
void world_update(float dt);

extern "C" {
#endif

extern int world_max_height;
extern draw_obj world_map;
int init_world(void);
void free_world(void);
void man_update(int man_idx, bool* pressed, vec3<float> curr_orientation);

#ifdef __cplusplus
}
#endif

#endif // GAME_WORLD_H
