#ifndef GAME_INIT_WORLD_H
#define GAME_INIT_WORLD_H


#ifdef __cplusplus
#include <game/man.h>
#include <math/vec3.h>
void add_player(man* player);
vec3<float> get_rand_coords();
extern "C" {
#endif
void in_skills();
void in_items();
//void in_armours();
//void in_animations();
int init_world(void);
void add_player(char *name, int clazz);
void free_world(void);

#ifdef __cplusplus
}
#endif

#endif // GAME_INIT_WORLD_H
