#ifndef MOD_T
#define MOD_T

struct mod_t {
    float int_mod, def_mod, atk_mod, spd_mod, hp_mod, frt_mod, agi_mod;
    mod_t tic(float t);
};

mod_t operator* (mod_t a, float b);
#endif // MOD_T
