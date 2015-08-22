#ifndef FIELD_H
#define FIELD_H


#include <cstdlib>
#include <vec2.h>

void render_line(vec2<int> p1, vec2<int> p2, bool **buf, bool val);
int get_rand(int l, int r);
bool** gen_field(int w, int h);

#endif // FIELD_H
