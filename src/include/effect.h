#ifndef EFFECT_H
#define EFFECT_H


#include <string>
<<<<<<< HEAD
#include <mod_type.h>

class effect {

public:
    std::string name;
    mod_t mods;
    float time;
    effect(std::string _name) : name(_name) {}
    mod_t tic(float t);
};


=======

class effect; {
    std::string name;

    effect(std::string _name) : name(_name) {}
};

>>>>>>> benny
#endif // EFFECT_H
