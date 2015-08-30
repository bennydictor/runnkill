#ifndef MOD_T
#define MOD_T

struct mod_t {
    float intellect, def_mod, atk_mod, speed, hp, mn, strength, agility;
    mod_t tic(float t);
};

mod_t operator* (mod_t a, float b);
#endif // MOD_T
