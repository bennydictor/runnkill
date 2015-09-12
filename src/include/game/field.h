#ifndef FIELD_H
#define FIELD_H
#define S_WIGHT 6
#define BIG_WIGHT 14

#include <math/vec2.h>
#include <cstdlib>
template <class T>
void render_line(vec2<int> p1, vec2<int> p2, T **buf, T val);
int get_rand(int l, int r);
bool** gen_field(int w, int h);
int** gen_field_sun(int w, int h);
int** gen_field_empty(int w, int h);
#endif // FIELD_H
