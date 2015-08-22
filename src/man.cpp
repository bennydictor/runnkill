#include <man.h>

using namespace std;
man::man(string _name, int cl) {
    name = _name;
    cls = cl;
    
    for (size_t i = 0; i < BP_AMOUNT; i++) {
        body_parts.push_back(bp_names[i]);
    }
    init_values(hp, mn, agility, strength, intellect, speed, cl);
    exp = level = 0;
}
