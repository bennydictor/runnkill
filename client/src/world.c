#include <world.h>

int world_w, world_h, render_distance = 20;
int **world_field;

float coords[3];
float speed[3];
float orientation[3];
float hp, mp, max_hp, max_mp, agility, strength, intellect, abs_speed, jump_high;
int level, EP, number;
char message[2048];
