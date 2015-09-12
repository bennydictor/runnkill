#ifndef GAME_BODY_PART_H
#define GAME_BODY_PART_H

#define LEFT_FRONT_UP 0 
#define LEFT_FRONT_DOWN 1
#define LEFT_BACK_UP 2
#define LEFT_BACK_DOWN 3
#define RIGHT_FRONT_UP 4
#define RIGHT_FRONT_DOWN 5
#define RIGHT_BACK_UP 6
#define RIGHT_BACK_DOWN 7
#include <game/items.h>
#include <string>

struct body_part {
    std::string name;
    float mod;
    item_t* item;
    bool is_fortified;
    body_part(std::string _name, float _mod);

    bool put_on(item_t* thing);
    item_t* put_off();
};

int count_dmg(body_part bp, int damage);
#endif // GAME_BODY_PART_H
