#ifndef MAN
#define MAN

#include <common.h>
#include <items.h>
#include <vector>
#include <string>
#include <body_part.h>
#include <effect.h>
#include <mod_type.h>
#include <skill_type.h>

class man {
    
public:
    std::string name;
    int cls;
    int level, exp;
    std::vector<item_t> bag;
    std::vector<body_part> body_parts;
    std::vector<effect> effects;
    std::vector<skill_t> skills;
    int hp, mn, agility, strength, intellect, speed, attack, deffense;
    man(std::string _name, int cl);
};
#endif // MAN
