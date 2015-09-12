#ifndef GAME_BULLET_H
#define GAME_BULLET_H

#include <iostream>
#include <game/effect.h>
#include <vector>
#include <string>
#include <math/vec3.h>

#define GRAVITATION 10


struct bullet {
    vec3<float> speed, coords;
    std::string name;
    std::vector<effect> effects;
    float damage;

    bullet();
    void in (std::istream& stream);
    vec3<float> in_time(float time);
};


#endif // GAME_BULLET_H
