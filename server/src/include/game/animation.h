#ifndef _GAME_ANIMATION_H_
#define _GAME_ANIMATION_H_

#include <vector>
#include <math/vec3.h>
#include <game/util/event.h>
#include <iostream>


struct animation {
    std::vector<event > events;
    event get(float t);
    void in(std::istream& stream);
};

#endif //_GAME_ANIMATION_H_
