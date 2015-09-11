#ifndef DRAW_OBJ
#define DRAW_OBJ

#include <vector>
#include <vec3.h>
#include <vertex3d.h>

#define SPHERE 1
#define ORTO 0
#define HEIGHT 16
#define HEIGHT1 17

#define WIDTH 16

extern std::vector<std::vector<vertex3d > > segments;
extern int sphere_ibo_data[8][HEIGHT * WIDTH][4];

class draw_obj {
  public:
    std::vector<vertex3d> points;
    std::vector<int> ibo_data;
    draw_obj();
    draw_obj(int t, ortohedron point);
    draw_obj(int t, float rad, vec3<float> coords);
};

void fill_segments();

#endif
