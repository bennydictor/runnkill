#ifndef ITEMS_H
#define ITEMS_H


#include <common.h>
#include <string>

class item_t {
    std::string type;
    int strength, agi, intellect, speed, defense;
    item_t(std::string _type) : type(_type) {}

public:
    bool is_wearable(std::string bp_name);
};

#endif // ITEMS_H
