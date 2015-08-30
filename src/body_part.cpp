#include <body_part.h>
#include <common.h>

using namespace std;

body_part::body_part(string _name, float _mod) {
    name = _name;
    is_fort = 0;
    mod = _mod;
    item = NULL;
}

bool body_part::put_on(item_t* thing) {
    if (thing->is_wearable(name)) {
        item = thing;
        return true;
    }
    return false;
}

item_t* body_part::put_off() {
    item_t* ret = item;
    item = NULL;
    return ret;
}

int count_dmg(body_part bp, int atk) {
    return bp.mod * (1 - 0.9 * bp.is_fort) * atk;
}
