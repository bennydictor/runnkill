#include <effects.h>
#include <mod_type.h>
using namespace std;

mod_t effects::tic(float t) {
    t = min(t, time);
    mod_t ret;
    ret = mods * t;
    time -= t;
    return ret;
}
