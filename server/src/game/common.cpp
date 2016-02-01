#include <game/common.h>

using namespace std;

/*
sring bp_names[] = {"head", "face", "body", "right_leg", "left_leg", "right_foot", "left_foot", // 7
            "right_arm", "left_arm", "right_hand", "left_hand", "neck", "back",                  // 6
            "right_finger_1", "right_finger_2", "right_finger_3", "right_finger_4",              // 4
            "left_finger_1",  "left_finger_2",  "left_finger_3",  "left_finger_4",               // 4
            "right_big_finger", "left_big_finger"};                                              // 2
*/
string bp_names[] = {"right down front", "left down front",
                     "right up front", "left up front",

                     "left down back", "right down back",
                     "left up back", "right back up"
};

map<string, string> item_bp;
float bp_init_mods[] = {
    0.7, 0.8, 1.3, 1.2, 
    0.8, 1,   1.8, 1.4, 
};
int cl_init_hp[] = {1500, 1100, 700};
int init_mp[] = {10, 20, 70};
int init_agi[] = {9, 15, 6};
int init_strength[] = {15, 9, 6};
int init_int[] = {3, 6, 15};
int init_speed[] = {4, 6, 5};
int init_jump_high[] = {4, 15, 4};
float init_attack_rad[] = {1.8, 1.5, 1.7};
int atk_strength_mods[] = {4, 2, 1};
int atk_agi_mods[] = {2, 3, 1};
int atk_int_mods[] = {0, 1, 5};
int exp_to_next_level[] = {25, 40, 80, 160, 320, 640, 1280};
map<string, int> to_idx;
map<string, map<string, int>> init_params;

long long count_score(int lv1, int lv2, int dmg, int heal) {
    return max(-1, (lv2 - lv1 + 1)) * max(dmg, max(heal, (dmg + heal) / 3));
}
void init_maps() {
    
    for (int i = 0; i < BP_AMOUNT; i++) {
        to_idx[bp_names[i]] = i;
    }
}

void init_values(float &hp, float &mp, float &agi, float &strength, float &intellect, float &speed, float& jump_high, float& attack_rad, int cl) {
    hp = cl_init_hp[cl];
    mp = init_mp[cl];
    agi = init_agi[cl];
    strength = init_strength[cl];
    intellect = init_int[cl];
    speed = init_speed[cl];
    jump_high = init_jump_high[cl];
    attack_rad = init_attack_rad[cl];
}


