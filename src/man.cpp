#include <common.h>
#include <items.h>
#include <vector>
#include <string>

using namespace std;
class man {
    string name, cls;
    int level, exp;
    vector<item_t> bag;
    vector<body_part> body_parts;
    vector<effect> effects;
    vector<skill_t> skills;
    int hp, mn, agility, strength, intellect, speed, attack, deffense;
    man(string _name, string cl) {
        name = _name;
        cls = cl;
        for (size_t i = 0; i < bp_names.size(); i++) {
            body_parts.push_back(bp_names[i]);
        }
        init_values(hp, mn, agility, strength, intellect, speed, cl);
        exp = level = 0;
    }
};
