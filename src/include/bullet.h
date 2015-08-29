#ifndef BULLET
#define BULLET

#include <effect.h>
#include <vector>
#include <string>

struct bullet {
    vec3 speed, coords;
    string type;
    vector<effect> effects;
    int damage;

    bullet() {
    }

    vec3 in_time(float time);
};





#endif //BULLET
