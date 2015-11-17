#ifndef GAME_INIT_WORLD_H
#define GAME_INIT_WORLD_H


#ifdef __cplusplus
extern "C" {
#endif

void in_skills();
void in_items();
void in_armours();
void in_animations();
int init_world(void);
void free_world(void);

#ifdef __cplusplus
}
#endif

#endif // GAME_INIT_WORLD_H
