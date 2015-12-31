#include <game/mod_type.h>

using namespace std;

mod_t operator* (const mod_t a, const float b) {
    mod_t ret;
    ret.hp = a.hp * b;
    ret.mp = a.mp * b;
    if (b == -1) {
        if (a.def_mod != 0)
            ret.def_mod = 1 / a.def_mod;
        if (a.atk_mod != 0)
            ret.atk_mod = 1 / a.atk_mod;
        if (a.speed != 0)
            ret.speed = 1 / a.speed;
        if (a.agility != 0)
            ret.agility = 1 / a.agility;
        if (a.intellect != 0)
            ret.intellect = 1 / a.intellect;
        if (a.strength != 0)
            ret.strength = 1 / a.strength;
    }
    return ret;
}
