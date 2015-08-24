#include <effect.h>
#include <mod_type.h>
using namespace std;

mod_t effect::tic(float t) {
    t = min(t, time);
    mod_t ret;
    ret = mods * t;
    time -= t;
    return ret;
}
