#include <mod_type.h>

using namespace std;

mod_t operator* (mod_t a, float b) {
    mod_t ret;
    ret.def_mod = a.def_mod * b;
    ret.atk_mod = a.atk_mod * b;
    ret.spd_mod = a.spd_mod * b;
    ret.agi_mod = a.agi_mod * b;
    ret.int_mod = a.int_mod * b;
    ret.hp_mod = a.hp_mod * b;
    return ret;
}
