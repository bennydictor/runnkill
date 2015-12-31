#ifndef GAME_FIELD_H
#define GAME_FIELD_H
#define S_WIGHT 5
#define BIG_WIGHT 10
#define UGLY_C 8
#define UGLY_DEPTH 2
#include <math/vec2.h>
#include <math/constants.h>
#include <cstdlib>
template <class T>
void render_line(vec2<int> p1, vec2<int> p2, T **buf, T val);
int get_rand(int l, int r);
bool** gen_field(int w, int h);
int** gen_field_sun(int w, int h);
int** gen_field_suns(int w, int h);
int** gen_field_lsuns(int w, int h);
int** gen_field_empty(int w, int h);
#endif // GAME_FIELD_H
