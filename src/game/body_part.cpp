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

item_t* body_part::put_on(item_t* thing) {
    item_t* ret = item;
    item = thing;
    return ret;
}

item_t* body_part::put_off() {
    item_t* ret = item;
    item = NULL;
    return ret;
}

int count_dmg(body_part bp, int atk) {
    float ret;
    if (bp.is_fortified)
        ret = bp.mod * 0.1 * atk;
    else if (bp.item)
        ret = bp.mod * bp.item->defense_coeff * atk;
    else
        ret = bp.mod * atk;
    cout << bp.name << ' ' << ret << endl;
    return (int)ret;
}
