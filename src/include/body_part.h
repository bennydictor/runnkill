#ifndef BODY_PART_H
#define BODY_PART_H

#include <items.h>
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
#endif // BODY_PART_H
