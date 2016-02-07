#ifndef GAME_MOD_TYPE_H
#define GAME_MOD_TYPE_H

struct mod_t {
    float intellect, def_mod, atk_mod, speed, hp, mp, strength, agility;
    int owner;
    mod_t();

    mod_t tic(float t);
};

mod_t operator* (mod_t a, float b);
#endif // GAME_MOD_TYPE_H
