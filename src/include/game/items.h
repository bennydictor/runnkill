#ifndef GAME_ITEMS_H
#define GAME_ITEMS_H

#include <game/mod_type.h>
#include <game/common.h>
#include <string>

struct item_t {
    std::string type;
    mod_t effects;
    int strength, agi, intellect, speed;
    float defense_coeff;
    item_t(std::string _type) : type(_type) {}
    
};

#endif // GAME_ITEMS_H
