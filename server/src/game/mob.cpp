#include <game/mob.h>
#include <game/man.h>
#include <game/field.h>
#include <math/vec3.h>
#include <math/geom.h>
#include <math/constants.h>
#include <vector>

using namespace std;

#define START_MOB_IDX 1024

int MOB_IDX;

mob::mob() {
    cls = 3;
    def_mod = atk_mod = 1; 
    is_alive = 2;
    can_die = true;
    have_shield = false;
    is_stunned = 0;
    touch_ground = false;
    my_aura = NULL;
//    weapon = NULL;
    orientation = vec3<float>(1, 0, 0);
    for (size_t i = 0; i < BP_AMOUNT; i++) {
        body_parts.push_back(body_part(bp_names[i], bp_init_mods[i]));
    }
    init_values(hp, mp, agility, strength, intellect, abs_speed, jump_high, attack_rad, cls);
    max_hp = hp;
    max_mp = mp;
    curr_skill = -1;

    exp = level = 0;
    level_exp = exp_to_next_level[0];
    number = GAME_MAX_MAN_IDX + START_MOB_IDX + MOB_IDX++;
    recovery.hp = 10;
    recovery.mp = 3;
}

void mob::what_to_do() {
    if (visible.empty() or is_stunned) {
        return;
    }
    if (touch_ground) {
        speed = vec3<float>(0, 0, 0);
    }
    bool using_attack = false;
    float min_hp = INF;
    man* to_move = NULL;
    for (man* z : visible) {
        float distance = dist(coords, z->coords);
        if (distance < attack_rad && skills[0]->time_to_activate_skill <= EPS_FOR_SKILLS) {
            orientation = vec3<float>(coords, z->coords);
            using_attack = true;
        } else if (z->coords.y > 0 and min_hp > z->hp * distance) {
            min_hp = z->hp * distance;
            to_move = z;
        }
    }
    orientation.resize(1);
    if (using_attack) {
        curr_skill = 0;
        skills[0]->time_to_activate_skill = skills[0]->between_activate_skill;
        need_to_cast = true;
        mp -= skills[0]->cost.mp;
    } else if (to_move) {
        if (touch_ground) {
            speed = vec3<float>(coords, to_move->coords);
            if (dist(coords, to_move->coords) > 8) {
                speed.x += get_rand(-2, 2);
                speed.z += get_rand(-2, 2);
            } else if (dist(coords, to_move->coords) < 1) {
                speed = -1.0f * speed;
            }
            if (get_rand(1, 6) == 1) {
                speed.y += jump_high;
            }
            speed.resize(abs_speed);
            
        }
    } else {
        speed.rotate(0.01, 0, 0);
    }

    visible.clear();
}

void mob::try_to_find(man* z) {
    if (dist(z->coords, coords) < MOB_VISIBLE_DIST) {
        visible.push_back(z);
    }
}
