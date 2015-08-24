#include <body_part.h>
<<<<<<< HEAD
#include <common.h>
=======

>>>>>>> benny

using namespace std;

body_part::body_part(string _name) {
    name = _name;
<<<<<<< HEAD
    hp = bp_init_hp[to_idx[_name]];
=======
    hp = init_hp[_name];
>>>>>>> benny
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
