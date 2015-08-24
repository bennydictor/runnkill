#include <common.h>

using namespace std;

string bp_names[] = {"head", "face", "body", "right_leg", "left_leg", "right_foot", "left_foot", // 7
            "right_arm", "left_arm", "right_hand", "left_hand", "neck", "back",                  // 6
            "right_finger_1", "right_finger_2", "right_finger_3", "right_finger_4",              // 4
            "left_finger_1",  "left_finger_2",  "left_finger_3",  "left_finger_4",               // 4
            "right_big_finger", "left_big_finger"};                                              // 2

map<string, string> item_bp;
int bp_init_hp[] = {
    10, 10, 10, 10, 10, // 5
    10, 10, 10, 10, 10, // 5
    10, 10, 10, 10, 10, // 5
    10, 10, 10, 10, 10, // 5
    10, 10, 10,
};
int cl_init_hp[] = {150, 110, 70};
int init_mn[] = {10, 20, 70};
int init_agi[] = {9, 15, 6};
int init_strength[] = {15, 9, 6};
int init_int[] = {3, 6, 15};
int init_speed[] = {4, 6, 5};
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

void init_values(int &hp, int &mn, int &agi, int &strength, int &intellect, int &speed, int cl) {
    hp = cl_init_hp[cl];
    mn = init_mn[cl];
    agi = init_agi[cl];
    strength = init_strength[cl];
    intellect = init_int[cl];
    speed = init_speed[cl];
}
