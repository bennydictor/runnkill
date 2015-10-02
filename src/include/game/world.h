#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include <graphics/draw_obj.h>

#ifdef __cplusplus

#include <vector>

void world_draw_objs(std::vector<draw_obj> &res);
void world_update(float dt);

extern "C" {
#endif


int init_world(void);
void free_world(void);


#ifdef __cplusplus
}
#endif

#endif // GAME_WORLD_H
