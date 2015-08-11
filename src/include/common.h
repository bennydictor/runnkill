#ifndef COMMON_H
#define COMMON_H


#include <string>
#include <map>

extern std::string bp_names[];
extern std::map<std::string, std::string> item_bp;
extern std::map<std::string, int> init_hp;

extern std::map<std::string, std::map<std::string, int>> init_params;

long long count_score(int lv1, int lv2);

#endif // COMMON_H
