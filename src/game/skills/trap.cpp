#include <game/skills/trap.h>

using namespace std;

trap::trap(vec3<float> _centre, float _rad, float _dmg, float t, int i) {
    centre = _centre;
    rad = _rad;
    dmg = _dmg;
    activate_time = t;
    time = 0;
    is_alive = true;
    material_idx = i;
}

bool trap::is_intersect(vec3<float> c, float r) {
//    cout << time << ' ' << activate_time << endl;
    if (vec3<float>(c, centre).sqlen() > (r + rad) * (r + rad)) {
        return false;
    }
    cout << "Капкан!" << endl;
    return true;
}
