#ifndef BULLET
#define BULLET

#include <effect.h>
#include <vector>
#include <string>
#include <vec2.h>
struct bullet {
    vec2<float> speed, coords;
    std::string type;
    std::vector<effect> effects;
    int damage;

    bullet() {
    }

    vec2<float> in_time(float time);
};





#endif //BULLET
