#include <draw_obj.h>
#include <vector>
#include <vec3.h>

using namespace std;

vector<vector<vec3<float> > > segments;

draw_obj::draw_obj(int t) {
}

template <class T>
draw_obj::draw_obj(int t, T rad, vec3<T> coords) {
    if (t == SPHERE) {
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 2; k++) {
                    points.append(segments[i][j][k]);
                }
            }
        }
    }
    for (int i = 0; i < points.size(); i++) {
        points[i] = points[i] * rad + coords;
    }
}

void fill_segments() {

    
}
