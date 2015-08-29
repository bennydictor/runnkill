#include <mod_type.h>

using namespace std;

mod_t operator* (const mod_t a, const float b) {
    mod_t ret;
    ret.def_mod = a.def_mod * b;
    ret.atk_mod = a.atk_mod * b;
    ret.speed = a.speed * b;
    ret.agility = a.agility * b;
    ret.intellect = a.intellect * b;
    ret.strength = a.strength * b;
    ret.hp = a.hp * b;
    ret.mn = a.mn * b;
    return ret;
}
