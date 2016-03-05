#ifndef GAME_MOB_H
#define GAME_MOB_H

#include <game/man.h>
#include <vector>

#define MOB_VISIBLE_DIST 15

struct mob : public man {
    mob();
    std::vector<man*> visible;
    void try_to_find(man* z);
    void what_to_do();
};

#endif //GAME_MOB_H
