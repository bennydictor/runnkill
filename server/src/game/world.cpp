#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstring>
#include <cassert>
#include <game/world.h>
#include <game/init_world.h>
#include <game/man.h>
#include <game/field.h>
#include <game/skills/skill_type.h>
#include <game/skills/bullet.h>
#include <game/skills/trap.h>
#include <game/skills/explosion.h>
//#include <game/armour.h>
#include <game/animation.h>
#include <game/util/event.h>
#include <math/geom.h>
#include <util/logstream.h>
#include <util/smth.h>
/*
#include <graphics/objects/box.h>
#include <graphics/objects/field.h>
#include <graphics/objects/sphere.h>
#include <graphics/objects/sphere_sector.h>
*/
#include <cassert>
#define INF 10000000
#define VEC3F vec3<float>()
#define EXP_CONSTANT 20
using namespace std;

const int len = 1;
int world_max_height;
draw_obj **world_map;
light_t gl_light[LIGHT_COUNT];
char gl_light_enable[LIGHT_COUNT];
draw_obj draw_objs[MAX_DRAW_OBJ];
int draw_obj_count;

map<int, man*> get_by_id;
map<int, int> exp_add;
vector<bullet> bullets;
vector<trap> traps;
vector<man*> persons;
vector<char> is_alive;
vector<bool>is_bullet_alive;
vector<explosion> explosions;
vector<vector<skill_t > > default_skills;
vector<item_t> default_items;
//vector<armour> default_armours;
vector<animation> animations;
vector<int> fake_materials_idx;
vec3<float> sector_points_a[8] = {
        vec3<float>(0, -1, 0), vec3<float>(0, -1, 0), vec3<float>(0, 1, 0), vec3<float>(0, 1, 0), 
        vec3<float>(0, -1, 0), vec3<float>(0, -1, 0), vec3<float>(0, 1, 0), vec3<float>(0, 1, 0), 
};
vec3<float> sector_points_b[8] = {
        vec3<float>(1, 0, 0), vec3<float>(1, 0, 0), vec3<float>(1, 0, 0), vec3<float>(1, 0, 0),
        vec3<float>(-1, 0, 0), vec3<float>(-1, 0, 0), vec3<float>(-1, 0, 0), vec3<float>(-1, 0, 0)
};
vec3<float> sector_points_c[8] = {
        vec3<float>(0, 0, 1), vec3<float>(0, 0, -1), vec3<float>(0, 0, 1), vec3<float>(0, 0, -1),
        vec3<float>(0, 0, 1), vec3<float>(0, 0, -1), vec3<float>(0, 0, 1), vec3<float>(0, 0, -1)
};
int** world_field;
int world_w, world_h;

draw_obj make_draw_sphere(vec3f pos, float rad, short int material) {
    draw_obj ret;
    ret.type = DRAW_SPHERE;
    memcpy(ret.pos, pos, 3 * sizeof(float));
    ret.rad = rad;
    ret.material = material;
    return ret;
}

draw_obj make_draw_sphere_sector(vec3f pos, float rad, float rot, int s, short int material) {
    draw_obj ret;
    ret.type = DRAW_SPHERE_SECTOR;
    memcpy(ret.pos, pos, 3 * sizeof(float));
    ret.rad = rad;
    ret.rot = rot;
    ret.s = s;
    ret.material = material;
    return ret;
}

template <class T>

vec3<T> get_turned(vec3<T> s, float angle) {
    vec3<T> res(s);
    res.x = cos(angle) * s.x - sin(angle) * s.z;
    res.z = sin(angle) * s.x + cos(angle) * s.z;
    return res;
}

template <class T>

int detect_sector(vec3<T> centre, vec3<T> point, vec3<T> orientation) {
    point.x -= centre.x;
    point.y -= centre.y;
    point.z -= centre.z;
    point = get_turned(point, atan2(orientation.x, orientation.z));
    float x = point.x, y = point.y, z = point.z;
    int ret[2][2][2] = {
        {{LEFT_BACK_DOWN, RIGHT_BACK_DOWN}, {LEFT_BACK_UP, RIGHT_BACK_UP}},
        {{LEFT_FRONT_DOWN, RIGHT_FRONT_DOWN}, {LEFT_FRONT_UP, RIGHT_FRONT_UP}},
    };
    string names[2][2][2] = {
        {{"LEFT_BACK_DOWN", "RIGHT_BACK_DOWN"}, {"LEFT_BACK_UP", "RIGHT_BACK_UP"}},
        {{"LEFT_FRONT_DOWN", "RIGHT_FRONT_DOWN"}, {"LEFT_FRONT_UP", "RIGHT_FRONT_UP"}},
    };
    //cout << "your number is " << names[x > 0][y > 0][z > 0] << endl;
    return ret[x < 0][y > 0][z < 0];
}

void add_exp(man* p, int e) {
    for (pair<int, int> i : p->healers)  {
        exp_add[i.first] += (i.second / p->max_hp) * e;
    }
}

bool is_intersected(vec3<float> centre, float rad, float rad2, vec3<float> begin, vec3<float>& end, vec3<float>& res) {
    vec3<float> l, r, m1, m2, m;
    l = begin;
    
    r = end;
    for (int i = 0; i < 10; i++) {
        m1 = ((float)2 * l + r) / (float)3;
        m2 = (l + (float)2 * r) / (float)3;
        if (dist(centre, m1) > dist(centre, m2)) {
            l = m1;
        } else {
            r = m2;
        }
    }
    if (dist(centre, l) >= rad + rad2 or dist(centre, l) < EPS_FOR_MOVE) {
        return false;
    }
    l = begin;
    for (int i = 0; i < 10; i++) {
        m = (l + r) / (float)2;
        if (dist(centre, m) < rad + rad2) {
            r = m;
        } else {
            l = m;
        }
    }
    vec3<float> end_vec(begin, l);
    if (end_vec.sqlen() < EPS_FOR_MOVE) {
        end = l;
        res = vec3<float>(end, centre);
        res.resize(rad2);
        res = res + end;
        return true;
    }
    end_vec.resize(sqrt(end_vec.sqlen()) - 10 * EPS_FOR_MOVE);
    end = begin + end_vec;
    res = vec3<float>(end, centre);
    res.resize(rad2);
    res = res + end;
    return true;
}
/*
int all_dmg(body_part u_l_bp, body_part u_r_bp, body_part d_l_bp, body_part d_r_bp, skill_t skill, int attack) {
    cerr << "We want to count damage!" << endl;
    return count_dmg(u_l_bp, skill.u_l * attack) + count_dmg(u_r_bp, skill.u_r * attack) 
         + count_dmg(d_l_bp, skill.d_l * attack) + count_dmg(d_r_bp, skill.d_r * attack);

}
*/

bool _in_sector(vec3<float> centre, vec3<float> a, vec3<float> b, vec3<float> c, vec3<float> p)
{
    vec3<float> pl_ab, pl_bc, pl_ac;
    float d_ab, d_bc, d_ac;
    pl_ab = plain(centre, a, b); // Мы строим три грани трёхгранного угла.
    d_ab = -pl_ab.dot(centre);
    pl_bc = plain(centre, c, b);
    d_bc = -pl_bc.dot(centre);
    pl_ac = plain(centre, a, c);
    d_ac = -pl_ac.dot(centre);
    return dist_to_plain(pl_ab, d_ab, c) * dist_to_plain(pl_ab, d_ab, p) >= 0 and
           dist_to_plain(pl_bc, d_bc, a) * dist_to_plain(pl_bc, d_bc, p) >= 0 and
           dist_to_plain(pl_ac, d_ac, b) * dist_to_plain(pl_ac, d_ac, p) >= 0;
}

bool in_sector(vec3<float> centre, int i, vec3<float> orientation, vec3<float> point)
{
    vec3<float> a = sector_points_a[i], b = sector_points_b[i], c = sector_points_c[i];
    float s = atan2(orientation.x, orientation.z);
    a.rotate(s);
    b.rotate(s);
    c.rotate(s);
    //cout <<"Our Sector has " << a << ' ' << b << ' ' << c << endl;
    return _in_sector(centre, a + centre, b + centre, c + centre, point);
}

bool intersect_sector_ball(vec3<float> centre, float rad1, float rad2, int i, vec3<float> orientation, vec3<float> begin, vec3<float>& end,vec3<float>& res)
{
    vec3<float> l, r, m1, m2, m;
    l = begin;
    r = end;
    for (int _ = 0; _ < 20; _++) {
        m1 = ((float)2 * l + r) / (float)3;
        m2 = ((float)2 * r + l) / (float)3;
        if (dist(centre, m1) > dist(centre, m2))
            l = m1;
        else
            r = m2;
    }
    if (dist(centre, l) >= rad1 + rad2 - EPS_FOR_MOVE or dist(centre, l) < EPS_FOR_MOVE) {
        return false;
    }
    l = begin;
    vec3<float> last_r = r;
    for (int _ = 0; _ < 15; _++) {
        m = (l + r) / (float)2;
        if (dist(centre, m) < rad1 + rad2)
            r = m;
        else
            l = m;
    }
    if (in_sector(centre, i, orientation, l))
    {
        res = l;
        return true;
    }
    l = last_r;
    r = end;

    for (int _ = 0; _ < 15; _++) {
        m = (l + r) / (float)2;
        if (dist(centre, m) > rad1 + rad2)
            r = m;
        else
            l = m;
    }
    if (in_sector(centre, i, orientation, l))
    {
        res = l;
        return true;
    }
    return false;
}

void damage_last_explosion() {
    //cout << explosions.back().first << endl;
    for (int j = 0; j < (int)persons.size(); j++) {
        //cout << dist(explosions.back().coords, persons[j]->coords) << ' ' << MAN_RAD << ' ' << explosions.back().rad << endl;
        if (dist(explosions.back().coords, persons[j]->coords) < MAN_RAD + explosions.back().rad) {
            int sector = detect_sector(persons[j]->coords, explosions.back().coords, persons[j]->orientation);
            is_alive[j] = !persons[j]->take_damage(
                        count_dmg(persons[j]->body_parts[sector], explosions.back().damage), explosions.back().owner);
            is_alive[j]++;
            if (is_alive[j] == 2) {
                for (int i = 0; i < (int)explosions.back().effects.size(); i++) {
                    persons[j]->add_effect((explosions.back().effects[i]));
                }
            }
        }
    }
    for (int j = 0; j < (int)traps.size(); j++) {
        if (traps[j].is_intersect(explosions.back().coords, explosions.back().rad))
            traps[j].is_alive = 0;
    }

}
int get_element(int** __F, int i, int j) {
    if (i < 0 or i > world_w - 1 or j < 0 or j > world_h - 1)
        return -INF;
    i = min(world_w - 1, max(i, 0));
    j = min(world_h - 1, max(j, 0));
    return __F[i][j];
}
template <class T>

bool move_sphere(vec3<T> start, vec3<T> &finish, T rad, int owner, bool Flag, vec3<T>& touch) {
    
    vec3<float>intersection = finish, curr_intersection;
    bool res = false;
    curr_intersection = finish;
    vec3<float> curr_finish = finish;
    for (int i = 0; i < (int)persons.size(); i++) {
        if (is_alive[i] == 2 && persons[i]->number != owner)
        {
            if (is_intersected(persons[i]->coords, rad, MAN_RAD, start,
                               curr_finish, curr_intersection)) {
                res = true;
                intersection = curr_intersection;
            }
            for (int j = 0; Flag and j < BP_AMOUNT; j++)
            {
                if (persons[i]->body_parts[j].is_fortified and 
                    intersect_sector_ball(persons[i]->coords, rad, MAN_RAD * 1.5, j,
                                          persons[i]->orientation, start, curr_finish, curr_intersection)) {
                    
                    res = true;
                    intersection = curr_intersection;
                }
            }
        }
    }
    vec3<float> point1, point2, point3, point4;
    for (int k = 0; k < 1; k++) {
        for (int i = -(int)rad - 1; i <= (int)rad + 1; i++) {
            for (int j = -(int)rad - 1; j <= (int)rad + 1; j++) {
                point1 = vec3<float>((int)finish.x + i - EPS_FOR_MOVE, -SMALL_INF, (int)finish.z - EPS_FOR_MOVE + j);
                point2 = vec3<float>((int)finish.x + 1 + EPS_FOR_MOVE + i, -SMALL_INF, (int)finish.z - EPS_FOR_MOVE + j);
                point3 = vec3<float>((int)finish.x - EPS_FOR_MOVE + i, get_element(world_field, (int)finish.x + i, (int)finish.z + j) + EPS_FOR_MOVE, 
                                                                    (int)finish.z - EPS_FOR_MOVE + j);
                point4 = vec3<float>((int)finish.x - EPS_FOR_MOVE + i, -SMALL_INF, (int)finish.z + 1 + EPS_FOR_MOVE + j);
                ortohedron curr(point1, point2, point3, point4);
                bool res1 = intersect_segment_sphere_ortohedron(
                                curr, start, curr_finish, rad + (float)EPS_FOR_MOVE, intersection);
                res |= res1;
                /*
                if (res)
                    cout << point1 << point2 << point3 << point4 << finish << endl;

                cout << res << endl;
                */
            }
        }
    }
    finish = curr_finish;
    touch = intersection;
    //cout << touch << endl;
    return res;
}

bool move_bullet(int b_idx, float time) {
        assert(time >= 0 or time <= 0);
    if (!is_bullet_alive[b_idx]) {
        return false;
    }
    if (abs(bullets[b_idx].coords.y) > 100) {
        explosions.push_back(explosion(bullets[b_idx].coords, EXPLOSION_TIME, bullets[b_idx].exp_rad, bullets[b_idx].damage));
        explosions.back().owner = bullets[b_idx].owner;
        for (int j = 0; j < (int)bullets[b_idx].effects.size(); j++) {
            explosions.back().effects.push_back(bullets[b_idx].effects[j]);
        }
        damage_last_explosion();
        is_bullet_alive[b_idx] = 0;
        return false;
    }
        
    vec3<float>our_point = bullets[b_idx].in_time(time);
    vec3<float> touch_point;
    bool res = move_sphere(bullets[b_idx].coords, our_point, (float)bullets[b_idx].rad, bullets[b_idx].owner, true, touch_point);
    if (res) {
//        cerr << "Strike #" << 179 << endl;
        
        explosions.push_back(explosion(touch_point, EXPLOSION_TIME, bullets[b_idx].exp_rad, bullets[b_idx].damage));
        explosions.back().owner = bullets[b_idx].owner;
        for (int j = 0; j < (int)bullets[b_idx].effects.size(); j++) {
            explosions.back().effects.push_back(bullets[b_idx].effects[j]);
            explosions.back().effects.back().owner = bullets[b_idx].owner;
        }
        damage_last_explosion();
        is_bullet_alive[b_idx] = 0;
        return false;
    }
    bullets[b_idx].coords = our_point; 
    bullets[b_idx].speed.y -= GRAVITATION * time;
    return true;
}

bool move_man(int idx, float time, int depth = 0) {
        assert(time >= 0 or time <= 0);
    if (depth > 10) {
        return true;
    }
    if (time < 0)
        return true;
    assert(time >= 0);
    //cout << persons[idx]->hp << endl;
    vec3<float> finish = persons[idx]->in_time(time);
    float beg_dist = dist(persons[idx]->coords, finish);
    assert(beg_dist >= 0 or beg_dist <= 0);
    if (beg_dist < EPS_FOR_MOVE) {
        persons[idx]->speed.y -= (time) * GRAVITATION;
        persons[idx]->move(time);
        return true;
    }
    persons[idx]->touch_ground = false;
    vec3<float> touch_point(0, 0, 0);
    char res = move_sphere(persons[idx]->coords, finish, (float)(MAN_RAD), persons[idx]->number, false, touch_point);
    if (res == 1) {
            //cout << persons[idx]->coords << finish << persons[idx]->in_time(time) << endl;   
            //cout << persons[idx]->speed << endl;
        persons[idx]->coords = finish;
        vec3<float> our_plain = vec3<float>(persons[idx]->coords, touch_point);
        float d = -our_plain.dot(persons[idx]->coords);
        vec3<float> tmp_point = persons[idx]->coords + persons[idx]->speed, normal = our_plain;
        normal.resize(dist_to_plain(our_plain, d, tmp_point) * -2);
        float _res = dist_to_plain(our_plain, d, tmp_point) * dist_to_plain(our_plain, d, tmp_point + normal); 
        if (_res > EPS_FOR_MOVE)
            normal = -1.0f * normal;

        persons[idx]->speed = vec3<float>(persons[idx]->coords, tmp_point + normal);
        float time_2 =  time * (1 - (dist(persons[idx]->coords, finish) / beg_dist));
        assert(time_2 >= 0 or time_2 <= 0);
        persons[idx]->move(time - time_2);
        vec3<float> rvector(persons[idx]->coords, touch_point);
        persons[idx]->speed.y -= (time - time_2) * GRAVITATION;
        move_man(idx, time_2, depth + 1);
        rvector.resize(1);
        if (rvector.y - EPS_FOR_SKILLS <= -1)
            persons[idx]->touch_ground = true;
    //cout << (rvector.y - EPS_FOR_MOVE <= -1) << endl;
    } else if (res == -1) {
        persons[idx]->coords.y += get_rand(5, 15);
    } else {
        persons[idx]->coords = (finish);
        persons[idx]->move(time);
        persons[idx]->speed.y -= (time) * GRAVITATION;
        if (persons[idx]->coords.y - get_element(world_field, persons[idx]->coords.x, persons[idx]->coords.z) - MAN_RAD < EPS_FOR_MOVE)
            persons[idx]->touch_ground = true;
    }

    return !res;
}

void attack(int man_idx, int idx) {
    man* z = persons[man_idx];
    //cout << z->mp << ' ' << z->skills[idx].cost.mp << endl;
    //cout << z->busy << ' ' << z->skills.size() << ' ' << z->skills[idx].to_activate << ' ' << z->skills[idx].between_activate << endl;
    if (z->busy > 0 or (int)z->skills.size() <= idx or z->skills[idx].cost.mp > z->mp or z->skills[idx].to_activate > EPS_FOR_SKILLS) {
    //    cerr << "You missed!" << endl;
        return;
    }
    //cout << z->skills[idx].to_activate << ' ' << z->skills[idx].between_activate << endl;
    z->mp -= z->skills[idx].cost.mp;
    //cerr << "Well, " << endl;
    skill_t curr = z->skills[idx];
    z->busy += animations[curr.animation_idx].events[0].dt;
    z->curr_skill = idx;
    z->skills[idx].to_activate = z->skills[idx].between_activate;
    z->need_to_cast = true;
}

bool move_trap(int idx, float time) {
//    cout << traps[idx].time << ' ' << traps[idx].activate_time << endl;
    traps[idx].time += time;
    if (traps[idx].time < traps[idx].activate_time or !traps[idx].is_alive) {
        return true;
    }
    for (int i = 0; i < (int)persons.size(); i++) {
        if (traps[idx].is_intersect(persons[i]->coords, MAN_RAD)) {
            vec3<float> to(persons[i]->coords, traps[idx].centre);
            to.resize(MAN_RAD);
            explosions.push_back(explosion(persons[i]->coords + to, EXPLOSION_TIME, EXPLOSION_RADIUS, traps[idx].dmg));
            explosions.back().owner = traps[idx].owner; 
            for (int j = 0; j < (int)traps[idx].effects.size(); j++) {
                explosions.back().effects.push_back(traps[idx].effects[j]);
            }
            damage_last_explosion();
            traps[idx].is_alive = false;
        }
    }
    
    return traps[idx].is_alive;
}

void kill_person(int idx) {
    is_alive[idx] = false;
}

void *get_person_data_begin(int idx) {
    return (void *) (persons[idx]->coords);
}

void *get_person_data_end(int idx) {
    return ((char *)&persons[idx]->number);
}

char *get_person_text(int idx) {
    return (persons[idx]->get_text());
}
float get_person_business(int idx) {
    man* z = persons[idx];
    if (z->curr_skill == -1 or (!z->need_to_cast)) {
        return -1;
    }
    return max(0.0f, z->busy - z->skills[z->curr_skill].activate_time);
}

float get_person_max_business(int idx) {
    man* z = persons[idx];
    if (z->curr_skill == -1 or (!z->need_to_cast)) {
        return -1;
    }
    return animations[z->skills[z->curr_skill].animation_idx].events[0].dt;
}

void world_callback(void) {
    draw_obj_count = 0;
    for (int i = 0; i < (int)persons.size(); i++) {
        if (is_alive[i]) {
            draw_objs[draw_obj_count++] = make_draw_sphere(persons[i]->coords, MAN_RAD, man_material.id);
            
            for (int j = 0; j < (int)persons[i]->effects.size(); j++) {
                if (persons[i]->effects[j].time > 0) {
                    vec3<float> centre = persons[i]->coords;
                    centre.y += 0.6 + j * 0.3;
                    draw_objs[draw_obj_count++] = (make_draw_sphere(centre, 0.1, fake_materials_idx[persons[i]->effects[j].material_idx]));
                }
            }
            float alpha = atan2(-persons[i]->orientation.z, persons[i]->orientation.x) + M_PI;
            for (int j = 0; j < BP_AMOUNT; j++) {
                if (persons[i]->body_parts[j].is_fortified)
                    draw_objs[draw_obj_count++] = make_draw_sphere_sector(persons[i]->coords, alpha, 1.5 * MAN_RAD, j, shield_material.id);
                else if (persons[i]->body_parts[j].item)
                {
                    draw_objs[draw_obj_count++] = make_draw_sphere_sector(persons[i]->coords, alpha, 
                             1.1 * MAN_RAD, j, fake_materials_idx[persons[i]->body_parts[j].item->material_idx]);
                }
            }
            /*
            if (persons[i]->weapon) {
                int one_idx = persons[i]->curr_skill;
                vec3<float> point1, p1p2 = persons[i]->orientation,
                            normal = plain(vec3<float>(0, 0, 0), 
                                           persons[i]->orientation, vec3<float>(0, 1, 0));
                normal.resize(0.5);
                p1p2.y = 0;
                p1p2.resize(persons[i]->weapon->length());
                point1 = persons[i]->coords + p1p2;
                int an_idx = persons[i]->skills[one_idx].animation_idx; 
                if (one_idx != -1) {
                
                   animations[an_idx].events.push_back(event(vec3<float>(0, 1, 0), 
                                                        VEC3F - persons[i]->orientation,
                                                        atan2(persons[i]->orientation.x, persons[i]->orientation.z)));
                                                        //-2 * atan2(persons[i]->orientation.x, persons[i]->orientation.z)));
                    result.push_back(persons[i]->weapon->give_me_points(
                                   animations[an_idx].get(persons[i]->busy),
                                   event(point1, vec3<float>(), 
                                         -atan2(persons[i]->orientation.x, persons[i]->orientation.z))));
                    animations[an_idx].events.pop_back();
                }
                else result.push_back(persons[i]->weapon->give_me_points(event(persons[i]->coords + vec3<float>(0, 1, 0), 
                                                                         persons[i]->coords - persons[i]->orientation,
                                                          -atan2(persons[i]->orientation.x, persons[i]->orientation.z))));
//                result.push_back(persons[i]->weapon->give_me_points(vec3<float>(0, 0, 0), vec3<float>(1, 0, 0)));
            }
            */ 
        }
    }
    for (int i = 0; i < (int) explosions.size(); ++i) {
        draw_objs[draw_obj_count++] = make_draw_sphere(explosions[i].coords,
                                      explosions[i].rad * powf(explosions[i].time, 1.0 / 3), explosion_material.id);
    }
    
    for (int i = 0; i < (int)bullets.size(); i++) {
        if (is_bullet_alive[i])
        draw_objs[draw_obj_count++] = make_draw_sphere(bullets[i].coords, bullets[i].rad, bullet_material.id);
    }

    for (int i = 0; i < (int)traps.size(); i++) {
        if (traps[i].is_alive) 
            draw_objs[draw_obj_count++] = make_draw_sphere(traps[i].centre, traps[i].rad, fake_materials_idx[traps[i].material_idx]);
    }
}


void world_update(float dt) {
    assert(dt >= 0 or dt <= 0);
    vector<bullet> new_bullets;
    vector<bool> new_is_bullet_alive;

    for (int i = 0; i < (int)bullets.size(); i++) {
        if (is_bullet_alive[i]) {
            move_bullet(i, dt);
            new_bullets.push_back(bullets[i]);
            new_is_bullet_alive.push_back(is_bullet_alive[i]);
        }
    }
    //cout << bullets.size() << endl;
    bullets = new_bullets; 
    is_bullet_alive = new_is_bullet_alive;
    for (int i = 0; i < (int)persons.size(); i++) {
        persons[i]->get_exp(exp_add[persons[i]->number]);
        exp_add[persons[i]->number] = 0;
        get_by_id[persons[i]->number] = persons[i];
        if (is_alive[i] == 2) {
            if (persons[i]->coords.y < -100) {
                is_alive[i] = 1;
                continue;
            }
            move_man(i, dt);
            if (persons[i]->skills.size() < 1) {
                for (skill_t k : default_skills[persons[i]->cls])
                persons[i]->skills.push_back(k);
            }
        }
        if (persons[i]->hp < 0 and is_alive[i] == 2) {
            is_alive[i] = 1;
            persons[i]->hp = 0;
        }
    }
    vector<explosion> nexp = explosions;
    explosions.clear();
    for (int i = 0; i < (int)nexp.size(); ++i) {
        if (nexp[i].time - nexp[i].end_time < 0) {
            explosions.push_back(nexp[i]);
        }
    }
    vector<trap> ntraps;
    for (int i = 0; i < (int)traps.size(); i++) {
        if (traps[i].is_alive) {
            move_trap(i, dt);
            ntraps.push_back(traps[i]);
        }
    }
    traps = ntraps;
    for (int i = 0; i < (int)explosions.size(); i++) {
        explosions[i].time += dt;
    }
}   


void man_update(int man_idx, char* pressed, vec3<float> curr_orientation) {
    man* z = persons[man_idx];
    z->set_orientation(curr_orientation);
    vec3<float> move_orientation = curr_orientation;
    //move_orientation.y /= z->abs_speed / 2;
    if (is_alive[man_idx] == 1) {
        persons[man_idx]->hp = 0;
        cout << "take this expp" << endl;
        for (pair<int, int> i : persons[man_idx]->damagers) {
            cout << i.first << ' ' << EXP_CONSTANT * i.second << ' ' << persons[man_idx]->max_hp << endl;
            int EP = (EXP_CONSTANT * i.second) / persons[man_idx]->max_hp;
            exp_add[i.first] += EP; 
            add_exp(get_by_id[i.first], EP);
        }
        persons[man_idx]->damagers.clear();
        persons[man_idx]->healers.clear();
        is_alive[man_idx] = 0;
        return;

    }
    if (pressed[WORLD_BLOCK_UL]) {
        z->fortify(LEFT_FRONT_UP);
    }
    if (pressed[WORLD_BLOCK_UR]) {
        z->fortify(RIGHT_FRONT_UP);
    }
    if (pressed[WORLD_BLOCK_DL]) {
        z->fortify(LEFT_FRONT_DOWN);
    }
    if (pressed[WORLD_BLOCK_DR]) {
        z->fortify(RIGHT_FRONT_DOWN);
    }
    if (pressed[WORLD_MOVE_FORWARD_EVENT] and pressed[WORLD_MOVE_BACKWARD_EVENT])
        pressed[WORLD_MOVE_FORWARD_EVENT] = pressed[WORLD_MOVE_BACKWARD_EVENT] = false;
    if (pressed[WORLD_MOVE_LEFT_EVENT] and pressed[WORLD_MOVE_RIGHT_EVENT])
        pressed[WORLD_MOVE_LEFT_EVENT] = pressed[WORLD_MOVE_RIGHT_EVENT] = false;
    if (z->touch_ground) {
        if (0 == pressed[WORLD_MOVE_FORWARD_EVENT] + pressed[WORLD_MOVE_RIGHT_EVENT] + pressed[WORLD_MOVE_BACKWARD_EVENT] + pressed[WORLD_MOVE_LEFT_EVENT])
        {
            z->set_speed(vec3<float>(0, 0, 0));
        }
        else
        {
            float angle = (pressed[WORLD_MOVE_RIGHT_EVENT] + 2 * pressed[WORLD_MOVE_BACKWARD_EVENT] + 3 * pressed[WORLD_MOVE_LEFT_EVENT] + 4 * pressed[WORLD_MOVE_FORWARD_EVENT] * pressed[WORLD_MOVE_LEFT_EVENT]) * M_PI / 2;
            angle /= pressed[WORLD_MOVE_FORWARD_EVENT] + pressed[WORLD_MOVE_RIGHT_EVENT] + pressed[WORLD_MOVE_BACKWARD_EVENT] + pressed[WORLD_MOVE_LEFT_EVENT];
            z->set_speed((float)z->abs_speed * move_orientation);
            z->speed.rotate(angle);
        }
        if (pressed[WORLD_ATTACK_EVENT]) {
            z->speed.y += z->jump_high;
        }
    }
    if (pressed[WORLD_SYM_1]) {
        attack(man_idx, 0);
    }
    if (pressed[WORLD_SYM_2]) {
        attack(man_idx, 1);
    }
    if (pressed[WORLD_SYM_3]) {
        attack(man_idx, 2);
    }
    if (pressed[WORLD_SYM_4]) {
        attack(man_idx, 3);
    }
    if (pressed[WORLD_SYM_5]) {
        attack(man_idx, 4);
    }
    if (z->need_to_cast and z->curr_skill != -1 and fabs(z->busy - z->skills[z->curr_skill].activate_time) < EPS_FOR_SKILLS) {
        skill_t curr = z->skills[z->curr_skill];
        z->need_to_cast = false;
        if (curr.type == 'R') {
            bullets.push_back(bullet(curr.sample));
            bullets.back().coords = z->coords + ((float)MAN_RAD + 2 * (float)curr.sample.rad) * z->orientation;
            bullets.back().speed = vec3<float>(z->orientation);
            bullets.back().speed.resize(curr.sample.speed.x);
            bullets.back().speed = bullets.back().speed + z->speed;
            bullets.back().damage *= count_attack(*z);
            bullets.back().owner = z->number;
            bullets.back().exp_rad = curr.sample.exp_rad;
            is_bullet_alive.push_back(1);
            cerr << "You shoot" << endl;
        } else if (curr.type == 'M') {
            cerr << "you try to beat" << endl;
            for (int i = 0; i < (int)persons.size(); i++) {
                if (i != man_idx) {
                    vec3<float> to_him(z->coords, persons[i]->coords);
                    if (sqrt(to_him.sqlen()) > (2 * MAN_RAD + curr.distance)) {
                        cout << "too far" << endl;
                    } else {
                        to_him.resize(1);
                        float angle = atan2(to_him.x, to_him.z) - atan2(z->orientation.x, z->orientation.z);
                        if (angle < curr.left_angle or angle > curr.right_angle) {
                            cout << "miss" << endl;
                            return;
                        } else {
                            cout << "attack succeed" << endl;
                            vec3<float> point, to_me(persons[i]->coords, z->coords);
                            int sector;
                            to_me.resize(MAN_RAD);
//                            to_me.y += curr.height;
                            sector = detect_sector(persons[i]->coords, persons[i]->coords + to_me, persons[i]->orientation);
                            is_alive[i] = !persons[i]->take_damage(
                                    count_dmg(persons[i]->body_parts[sector], curr.dmg * count_attack(*z)), z->number);
                        }
                    }
                }
            }
        } else if (curr.type == 'T') {
            if (z->touch_ground) {
                vec3<float> centre = z->coords;
                centre.y -= MAN_RAD;
                traps.push_back(trap(centre, curr.distance, curr.dmg * count_attack(*z), curr.busy_time, curr.material_idx));
                traps.back().owner = z->number;
                for (int i = 0; i < (int)curr.effects.size(); i++) {
                    traps.back().effects.push_back(curr.effects[i]);
                    traps.back().effects.back().owner = z->number;
                }
                cout << "Охота началась!" << endl;
            }
        }
    }
}
