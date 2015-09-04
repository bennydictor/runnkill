#include <vector>
#include <effect.h>
#include <skill_type.h>

using namespace std;

void skill_t::in_damage(istream& stream) {
    if (is_range) {
        stream >> dmg;
        sample.in(stream);
        sample.damage = dmg;
    } else {
        stream >> u_l >> u_r >> d_l >> d_r;
    }
    int amount_of_effects;
    stream >> amount_of_effects;
    string eff_name;
    for (int i = 0; i < amount_of_effects; i++) {
        stream >> eff_name;
        effects.push_back(effect(eff_name));
        effects[i].in(stream);
    }
    if (is_range) {
        sample.effects = effects;
    }
}

