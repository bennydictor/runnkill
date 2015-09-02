#include <mod_type.h>

using namespace std;

mod_t operator* (const mod_t a, const float b) {
    mod_t ret;
    ret.hp = a.hp * b;
    ret.mp = a.mp * b;
    if (b == -1) {
        ret.def_mod = 1 / a.def_mod;
        ret.atk_mod = 1 / a.atk_mod;
        ret.speed = 1 / a.speed;
        ret.agility = 1 / a.agility;
        ret.intellect = 1 / a.intellect;
        ret.strength = 1 / a.strength;
    }
    return ret;
}
