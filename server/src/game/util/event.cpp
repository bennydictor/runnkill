#include <game/util/event.h>
#include <math/vec3.h>
#include <math/geom.h>
using namespace std;

event::event() {
    angle = 0;
    point2 = point1 = vec3<float>(0, 0, 0);
};

event::event(vec3<float> p1, vec3<float> p2, float a) {
    angle = a;
    point1 = p1;
    point2 = p2;
    dt = 0;
}

void event::in(istream& stream) {
    float x, y, z;
    stream >> x >> y >> z;
    point1 = vec3<float>(x, y, z);
    stream >> x >> y >> z >> angle >> dt;
    point2 = vec3<float>(x, y, z);
}

float event::get_dist() {
    return dist(point1, point2);
}

