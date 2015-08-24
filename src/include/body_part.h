#ifndef BODY_PART_H
#define BODY_PART_H

<<<<<<< HEAD
=======

>>>>>>> benny
#include <items.h>
#include <string>

class body_part {
    std::string name;
    int hp;
    item_t* item;
<<<<<<< HEAD
public:
=======

>>>>>>> benny
    body_part(std::string _name);

    bool put_on(item_t* thing);
    item_t* put_off();
};

#endif // BODY_PART_H
