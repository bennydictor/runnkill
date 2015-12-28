#ifndef GAME_INIT_WORLD_H
#define GAME_INIT_WORLD_H


#ifdef __cplusplus
extern "C" {
#endif

void in_skills();
void in_items();
int init_world(void);
void add_player(char *name, int clazz);
void free_world(void);

#ifdef __cplusplus
}
#endif

#endif // GAME_INIT_WORLD_H
