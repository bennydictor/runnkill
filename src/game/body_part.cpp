#include <game/body_part.h>
#include <game/common.h>
#include <iostream>

using namespace std;

body_part::body_part(string _name, float _mod) {
    name = _name;
    is_fortified = 0;
    mod = _mod;
    item = NULL;
}

void body_part::put_on(item_t* thing) {
    ret = item;
    item = thing;
    return ret;
}

item_t* body_part::put_off() {
    item_t* ret = item;
    item = NULL;
    return ret;
}

int count_dmg(body_part bp, int atk) {
    return bp.mod * (1 - 0.9 * bp.is_fortified) * atk;
}
