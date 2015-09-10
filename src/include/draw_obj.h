#ifndef DRAW_OBJ
#define DRAW_OBJ

#include <vector>
#include <vec3.h>

#define HEIGHT 16
#define WIDTH 16

extern std::vector<std::vector<vec3<float> > > segments;
extern int sphere_ibo_data[8][HEIGHT * WIDTH][4];

class draw_obj {
  public:
    std::vector<vertex3d> points;

    draw_obj();
    draw_obj(int t, ortohedron point);
    draw_obj(int t, float rad, vec3<float> coords);
};

void fill_segments();

#endif
