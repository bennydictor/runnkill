#include <game/skills/aura.h>
#include <game/skills/effect.h>
#include <vector>

using namespace std;
aura::aura(float t, float dt, float d, vector<effect>& e) {
    can_use = 0;
    tic = dt;
    time = t;
    distance = d;
    effects = e;
}

void aura::set_owner(int o) {
    for (effect& k : effects) {
        k.owner = o;
    }
}
