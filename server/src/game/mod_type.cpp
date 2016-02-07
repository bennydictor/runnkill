#include <game/mod_type.h>
#include <iostream>
#include <cmath>
#include <math/constants.h>
using namespace std;

mod_t::mod_t() {
    def_mod = atk_mod = 1;
}

mod_t operator* (const mod_t a, const float b) {
    mod_t ret;
    ret.hp = a.hp * b;
    ret.mp = a.mp * b;
    if (b == -1) {
        if (fabs(a.def_mod) > EPS_FOR_SKILLS)
            ret.def_mod = 1 / a.def_mod;
        if (fabs(a.atk_mod) > EPS_FOR_SKILLS)
            ret.atk_mod = 1 / a.atk_mod;
        if (fabs(a.speed) > EPS_FOR_SKILLS)
            ret.speed = 1 / a.speed;
        if (fabs(a.agility) > EPS_FOR_SKILLS)
            ret.agility = 1 / a.agility;
        if (fabs(a.intellect) > EPS_FOR_SKILLS)
            ret.intellect = 1 / a.intellect;
        if (fabs(a.strength) > EPS_FOR_SKILLS)
            ret.strength = 1 / a.strength;
    }
    return ret;
}
