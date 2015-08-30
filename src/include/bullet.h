#ifndef BULLET
#define BULLET

#include <effect.h>
#include <vector>
#include <string>
#include <vec3.h>
struct bullet {
    vec3<float> speed, coords;
    std::string name;
    std::vector<effect> effects;
    int damage;

    bullet() {
    }

    vec3<float> in_time(float time);
};





#endif //BULLET
