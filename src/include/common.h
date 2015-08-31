#ifndef COMMON_H
#define COMMON_H

#define BP_AMOUNT 8

#define LEFT_UP 0
#define LEFT_DOWN 2
#define RIGHT_UP 4
#define RIGHT_DOWN 6

#include <string>
#include <map>

extern std::string bp_names[];
extern std::map<std::string, std::string> item_bp;
extern std::map<std::string, int> to_idx;
extern int cl_init_hp[];
extern std::map<std::string, std::map<std::string, int>> init_params;
extern float bp_init_mods[];

extern int atk_strength_mods[];
extern int atk_agi_mods[];
extern int atk_int_mods[];

long long count_score(int lv1, int lv2, int dmg, int heal);
void init_values(int &hp, int &mn, int &agi, int &strength, int &intellect, int &speed, int cl);
#endif // COMMON_H
