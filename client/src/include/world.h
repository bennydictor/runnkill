#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#define WORLD_EVENT_COUNT 8
#define WORLD_MOVE_FORWARD_EVENT 0
#define WORLD_MOVE_RIGHT_EVENT 1
#define WORLD_MOVE_BACKWARD_EVENT 2
#define WORLD_MOVE_LEFT_EVENT 3
#define WORLD_ATTACK_EVENT 4
#define WORLD_SYM_1 5
#define WORLD_SYM_2 6
#define WORLD_SYM_3 7

#define LIGHT_COUNT 8

#ifdef __cplusplus
extern "C" {
#endif

#include <graphics/draw_obj.h>

#define DRAW_SPHERE 0
#define DRAW_SPHERE_SECTOR 1
#define MAX_DRAW_OBJ 256

extern int world_w, world_h, render_distance;
extern int **world_field;
extern float coords[3];
extern float speed[3];
extern float orientation[3];
extern float hp, mp, max_hp, max_mp, agility, strength, intellect, abs_speed, jump_high;
extern int EP, level, level_exp;
extern char message[2048];
#ifdef __cplusplus
}
#endif

#endif // GAME_WORLD_H
