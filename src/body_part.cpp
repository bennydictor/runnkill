#include <body_part.h>
#include <common.h>

using namespace std;

body_part::body_part(string _name) {
    name = _name;
    hp = bp_init_hp[to_idx[_name]];
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
