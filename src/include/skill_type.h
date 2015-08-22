#include <vector>
#include <effect.h>

#ifndef SKILL_TYPE
#define SKILL_TYPE


struct skill_t {
    std::string name;       
    std::vector<effect> effects;
};


#endif // SKILL_TYPE
