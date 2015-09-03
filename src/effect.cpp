#include <effect.h>
#include <mod_type.h>
using namespace std;

mod_t effect::tic(float t) {
    t = min(t, time);
    mod_t ret;
    ret = mods_one_side * t;
    time -= t;
    return ret;
}

void effect::in(istream& stream) {
    stream >> time;
    stream >> mods_one_side.hp >> mods_one_side.mp;
    stream >> mods_two_side.strength >> mods_two_side.agility >> mods_two_side.intellect >> mods_two_side.speed;
}
