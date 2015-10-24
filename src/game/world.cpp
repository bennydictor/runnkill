#include <game/world.h>
#include <iostream>
#include <game/man.h>
#include <game/field.h>
#include <cstdio>
#include <fstream>
#include <game/skill_type.h>
#include <game/bullet.h>
#include <math/geom.h>
#include <util/log.h>
#include <vector>
#include <cmath>
#include <ctime>
#include <graphics/objects/box.h>
#include <graphics/objects/field.h>
#include <graphics/objects/sphere.h>
#include <graphics/objects/sphere_sector.h>
#define EXPLOSION_RAD 1e-2
#define INF 10000000
using namespace std;

const int len = 1;
int world_max_height;
draw_obj world_map;

vector<bullet> bullets;
vector<man*> persons;
vector<bool> is_alive, alive_bullets;
vector<pair<vec3<float>, float> > explosions;
vector<vector<skill_t > > default_skills;
vector<item_t> default_items;
int** F;
int w, h;

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
    point = get_turned(point, M_PI / 4 + atan2(orientation.z, orientation.x));
    float x = point.x, y = point.y, z = point.z;
    if (x > 0) {
        if (y > 0) {
            if (z > 0) {
                return LEFT_FRONT_UP;
            } else {
                return RIGHT_FRONT_UP;
            }
        } else {
            if (z > 0) {
                return LEFT_FRONT_DOWN;
            } else {
                return RIGHT_FRONT_DOWN;
            }
        }
    } else {
        if (y > 0) {
            if (z > 0) {
                return LEFT_BACK_UP;
            } else {
                return RIGHT_BACK_UP;
            }
        } else {
            if (z > 0) {
                return LEFT_BACK_DOWN;
            } else {
                return RIGHT_BACK_DOWN;
            }
        }
    }

}

bool is_intersected(vec3<float> centre, float rad, float rad2, vec3<float> begin, vec3<float> end, vec3<float>& res) {
    vec3<float> l, r, m1, m2, m;
    l = begin;
    r = end;
    for (int i = 0; i < 40; i++) {
        m1 = ((float)2 * l + r) / (float)3;
        m2 = (l + (float)2 * r) / (float)3;
        if (dist(centre, m1) > dist(centre, m2)) {
            l = m1;
        } else {
            r = m2;
        }
    }
    if (dist(centre, l) >= rad + rad2 - EPS or dist(centre, l) < EPS) {
        return false;
    }
    l = begin;
    for (int i = 0; i < 30; i++) {
        m = (l + r) / (float)2;
        if (dist(centre, m) < rad + rad2) {
            r = m;
        } else {
            l = m;
        }
    }
    res = l;
    return true;
}

int all_dmg(body_part u_l_bp, body_part u_r_bp, body_part d_l_bp, body_part d_r_bp, skill_t skill, int attack) {
    cerr << "We want to count damage!" << endl;
    return count_dmg(u_l_bp, skill.u_l * attack) + count_dmg(u_r_bp, skill.u_r * attack) 
         + count_dmg(d_l_bp, skill.d_l * attack) + count_dmg(d_r_bp, skill.d_r * attack);

}
void damage_last_explosion(int b_idx) {
    for (int j = 0; j < (int)persons.size(); j++) {
        if (dist(explosions.back().first, persons[j]->coords) < MAN_RAD + explosions.back().second) {
            int sector = detect_sector(persons[j]->coords, explosions.back().first, persons[j]->orientation);
            is_alive[j] = !persons[j]->take_damage(
                        count_dmg(persons[j]->body_parts[sector], bullets[b_idx].damage));
            //Here will be effects adding
        }
    }

}
int get_element(int** __F, int i, int j) {
    if (i < 0 or i > w - 1 or j < 0 or j > h - 1)
        return 0;
    i = min(w - 1, max(i, 0));
    j = min(h - 1, max(j, 0));
    return __F[i][j];
}
template <class T>

bool move_sphere(vec3<T> start, vec3<T> &finish, T rad) {
    
    vec3<float>intersection = finish, curr_intersection;
    bool res = false;
    curr_intersection = finish;
    for (int i = 0; i < (int)persons.size(); i++) {
        if (is_intersected(persons[i]->coords, MAN_RAD, EXPLOSION_RAD, start,
            finish, curr_intersection)) {
            res = true;
            if (dist(start, intersection) > (dist(start, curr_intersection)))
                intersection = curr_intersection;
        }
    }
    vec3<float> point1, point2, point3, point4;
    for (int i = -(int)rad - 1; i <= (int)rad + 1; i++) {
        for (int j = -(int)rad - 1; j <= (int)rad + 1; j++) {
            point1 = vec3<float>((int)finish.x - rad + EPS + i, -rad + EPS, (int)finish.z - rad + EPS + j);
            point2 = vec3<float>((int)finish.x + 1 + rad - EPS + i, -rad + EPS, (int)finish.z - rad + EPS + j);
            point3 = vec3<float>((int)finish.x - rad + EPS + i, get_element(F, (int)finish.x + i, (int)finish.z + j) + rad - EPS, 
                                                                (int)finish.z - rad + EPS + j);
            point4 = vec3<float>((int)finish.x - rad + EPS + i, -rad + EPS, (int)finish.z + 1 + rad - EPS + j);
            //cout << finish << ' ' << point1 << ' ' << point2 << ' ' << point3 << ' ' << point4 << endl;
            bool res1 = intersect_seg_ortohedron(
                            ortohedron(point1, point2, point3, point4), start, finish, curr_intersection);
            //cerr << start << ' ' << res1 << endl;
            if (res1 and dist(start, intersection) > (dist(start, curr_intersection))) {
                

                (intersection = curr_intersection);
            }
            
            res |= res1;
        }
    }
    if (!res)
        finish = start +  (float)(0.99) * vec3<float>(start, intersection);   
    //cout << finish << endl;
    else
        finish = start;
    return res;
}

bool move_bullet(int b_idx, float time) {
    if (!alive_bullets[b_idx]) {
        return false;
    }
    if (bullets[b_idx].coords.y < -10) {
        explosions.push_back(make_pair(bullets[b_idx].coords, bullets[b_idx].exp_rad));
        damage_last_explosion(b_idx);
        alive_bullets[b_idx] = 0;
        return false;
    }
        
    vec3<float>our_point = bullets[b_idx].in_time(time);
    bool res = move_sphere(bullets[b_idx].coords, our_point, (float)bullets[b_idx].rad);
    if (res) {
        cerr << "Strike #" << 179 << endl;
        explosions.push_back(make_pair(our_point, bullets[b_idx].exp_rad));
        damage_last_explosion(b_idx);
        alive_bullets[b_idx] = 0;
        return false;
    }
    bullets[b_idx].coords = our_point; 
    bullets[b_idx].speed.y -= GRAVITATION * time;
    return true;
}

bool move_man(int idx, float time) {
    vec3<float> finish = persons[idx]->in_time(time);
    //cout << "--" << finish << endl;
    //cout << '-' << persons[idx]->coords << endl;
   // return false;
    persons[idx]->move(time);
    if (finish == persons[idx]->coords)
    {
       // cout << '!' << endl;
        return true;
    }
    bool res = move_sphere(persons[idx]->coords, finish, (float)(MAN_RAD));
    persons[idx]->coords = (finish);
    return !res;
}

void attack(int man_idx, int idx) {

    man* z = persons[man_idx];
    if (z->busy > 0 or (int)z->skills.size() <= idx or z->skills[idx].cost.mp > z->mp) {
        cerr << "You missed!" << endl;
        return;
    }
    cerr << "Well, " << endl;
    skill_t curr = z->skills[idx];
     if (curr.is_range) {
        z->busy += curr.busy_time;
        bullets.push_back(bullet(curr.sample));
        bullets.back().coords = z->coords + ((float)MAN_RAD + 2 * (float)curr.sample.rad) * z->orientation;
        bullets.back().speed = vec3<float>(z->coords, bullets.back().coords);
        bullets.back().speed.resize(sqrt(z->speed.sqlen()) + curr.sample.speed.x);
        bullets.back().damage *= count_attack(*z);
        alive_bullets.push_back(1);
        cerr << "You shoot" << endl;
    } else {
        z->busy += curr.busy_time;
        cerr << "you try to beat" << endl;
        for (size_t i = 0; i < persons.size(); i++) {
            if ((int)i != man_idx and dist(persons[i]->coords, z->coords) < len and 
                                    z->speed.dot(vec3<float>(z->coords, persons[i]->coords))) {
                cerr << "You want to attack man #" << i << endl;
                vec3<float> I, He;
                He.x = persons[i]->coords.x;
                He.y = persons[i]->coords.z;
                I.x = z->coords.x - He.x;
                I.y = z->coords.z - He.z;
                I = get_turned(I, M_PI / 4 + atan2(persons[i]->orientation.z, persons[i]->orientation.x));
                cerr << "Yeah, we can detected an attack direction!" << endl;
                int damage;
                int attack = count_attack(*z);
                if (He.x > I.x) {
                    if (He.z > I.z) {
                        cerr << "It`s 0" << endl;
                        damage = all_dmg(persons[i]->body_parts[LEFT_FRONT_UP], persons[i]->body_parts[RIGHT_FRONT_UP],
                                        persons[i]->body_parts[LEFT_FRONT_DOWN], persons[i]->body_parts[RIGHT_FRONT_DOWN],
                                        z->skills[idx], attack);
                    } else {
                        cerr << "It`s 1" << endl;
                        damage = all_dmg(persons[i]->body_parts[RIGHT_BACK_UP], persons[i]->body_parts[LEFT_BACK_UP],
                                        persons[i]->body_parts[RIGHT_BACK_DOWN], persons[i]->body_parts[LEFT_BACK_DOWN],
                                            z->skills[idx], attack);
                    }
                } else {
                    if (He.z > I.z) {
                        cerr << "It`s 2" << endl;
                        damage = all_dmg(persons[i]->body_parts[RIGHT_FRONT_UP], persons[i]->body_parts[RIGHT_BACK_UP],
                                        persons[i]->body_parts[RIGHT_FRONT_DOWN], persons[i]->body_parts[RIGHT_BACK_DOWN],
                                        z->skills[idx], attack);
                    } else {
                        cerr << "It`s 3" << endl;
                        damage = all_dmg(persons[i]->body_parts[LEFT_BACK_UP], persons[i]->body_parts[LEFT_FRONT_UP],
                                            persons[i]->body_parts[LEFT_BACK_DOWN], persons[i]->body_parts[LEFT_FRONT_DOWN],
                                            z->skills[idx], attack);
                    }
                }
                is_alive[i] = !persons[i]->take_damage(damage);
                //Here will be effects adding
            }
        }
    }
}


void world_callback(vector<draw_obj> &result, vec3f coord) {
    result.clear();
    /*for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            ortohedron bounds(vec3<float>(i, 0, j), vec3<float>(i + 1, 0, j), vec3<float>(i, F[i][j] + 1, j), vec3<float>(i, 0, j + 1));
            result.push_back(make_draw_box(bounds, default_material));
        }
    }*/
    //cout << persons.size() << endl;
    result.push_back(world_map);
    for (int i = 0; i < (int)persons.size(); i++) {
        if (is_alive[i]) {
            result.push_back(make_draw_sphere3fv1f(persons[i]->coords, MAN_RAD, man_material));
            //cout << persons[i]->coords << endl;
            for (int j = 0; j < BP_AMOUNT; j++) {
                if (persons[i]->body_parts[j].is_fortified)
                    result.push_back(make_draw_sphere_sector3fv1f(persons[i]->coords, 1.5 * MAN_RAD, j, shield_material));
                else if (persons[i]->body_parts[j].item)
                {
                    result.push_back(make_draw_sphere_sector3fv1f(persons[i]->coords, 8 * MAN_RAD, j, persons[i]->body_parts[j].item->material));
                }
            }
        }
    }
    
    //cout << BP_AMOUNT << endl;
    for (int i = 0; i < (int)bullets.size(); i++) {
        if (alive_bullets[i])
        result.push_back(make_draw_sphere3fv1f(bullets[i].coords, bullets[i].rad, bullet_material));
    }

    coord[0] = persons[0]->coords.x - 5 * (persons[0]->orientation.x);
    coord[1] = persons[0]->coords.y + (1 - persons[0]->orientation.y);
    coord[2] = persons[0]->coords.z - 5 * (persons[0]->orientation.z);
}

void in_skills() {
    fstream in;
    in.open("skills");
    string garbage;
    int amount;
    char type;
    default_skills.resize(3);
    for (int i = 0; i < 3; i++) {
        in >> garbage;
        in >> amount;
        default_skills[i].resize(amount);
        for (int j = 0; j < amount; j++) {
            in >> type;
            cout << '!' << type << endl;
            default_skills[i][j].is_range = (type == 'R');
            default_skills[i][j].in_damage(in);
        }
    }
    in.close();
}

void in_items() {
    fstream in;
    in.open("items");
    int am;
    in >> am;
    default_items.resize(am);
    for (int j = 0; j < am; j++) {
        default_items[j].in(in);
    }
    in.close();
}

int init_world(void) {
    w = h = 100;
    F = gen_field_sun(w, h);
    world_max_height = 0;
    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            world_max_height = max(world_max_height, F[i][j]);
        }
    }
    init_field_object(w, h, F);
    world_map = make_draw_field(default_material);
    in_skills();
    in_items();
    int i = rand() % w;
    int j = rand() % h;
    i = j = 1;
    while (F[i][j] != 0)
    {
        j++;
        if (j == h)
        {
            j = 0;
            i++;
        }
    }
    persons.push_back(new man("Derrior", 1));
    is_alive.push_back(1);
    persons[0]->coords = vec3<float>((float)i + 0.5, MAN_RAD, (float)j + 0.5);
    persons[0]->set_speed(vec3<float>(0, 0, 0));
    persons[0]->skills.push_back(default_skills[1][0]);
    persons[0]->body_parts[0].put_on(new item_t(default_items[0]));
    cout << persons[0]->skills.size() << endl;
    cout << persons[0]->skills[0].is_range << endl;
    cout << (default_skills[1][0].is_range) << endl;
    return 0;
}

void free_world(void) {
    free_field_object();
}

void world_update(float dt, char *evs, vec3f rot) {
    for (int i = 0; i < (int)bullets.size(); i++) {
        if (alive_bullets[i])
            move_bullet(i, dt);
    }
    for (int i = 0; i < (int)persons.size(); i++) {
        if (is_alive[i])
            move_man(i, dt);
    }
    man_update(0, evs, vec3<float>(sinf(rot[1]), -rot[0] * 2 + 0.1, -cosf(rot[1])));
}   


void man_update(int man_idx, char* pressed, vec3<float> curr_orientation) {
    persons[man_idx]->set_orientation(curr_orientation);
    vec3<float> move_orientation = curr_orientation;
    move_orientation.y = 0;
    //cout << persons[man_idx]->orientation << endl;
    if (!is_alive[man_idx])
        return;
    if (pressed[WORLD_MOVE_FORWARD_EVENT] and pressed[WORLD_MOVE_BACKWARD_EVENT])
        pressed[WORLD_MOVE_FORWARD_EVENT] = pressed[WORLD_MOVE_BACKWARD_EVENT] = false;
    if (pressed[WORLD_MOVE_LEFT_EVENT] and pressed[WORLD_MOVE_RIGHT_EVENT])
        pressed[WORLD_MOVE_LEFT_EVENT] = pressed[WORLD_MOVE_RIGHT_EVENT] = false;
    if (0 == pressed[WORLD_MOVE_FORWARD_EVENT] + pressed[WORLD_MOVE_RIGHT_EVENT] + pressed[WORLD_MOVE_BACKWARD_EVENT] + pressed[WORLD_MOVE_LEFT_EVENT])
    {
        persons[man_idx]->set_speed(vec3<float>(0, 0, 0));
    }
    else
    {
        float angle = (pressed[WORLD_MOVE_RIGHT_EVENT] + 2 * pressed[WORLD_MOVE_BACKWARD_EVENT] + 3 * pressed[WORLD_MOVE_LEFT_EVENT] + 4 * pressed[WORLD_MOVE_FORWARD_EVENT] * pressed[WORLD_MOVE_LEFT_EVENT]) * M_PI / 2;
        angle /= pressed[WORLD_MOVE_FORWARD_EVENT] + pressed[WORLD_MOVE_RIGHT_EVENT] + pressed[WORLD_MOVE_BACKWARD_EVENT] + pressed[WORLD_MOVE_LEFT_EVENT];
        persons[man_idx]->set_speed((float)persons[man_idx]->abs_speed * move_orientation);
        //cout << "V " << persons[man_idx]->speed << endl;
        persons[man_idx]->speed.rotate(angle);
        //cout << "v " << (angle > M_PI) << ' ' << persons[man_idx]->speed << endl;
        //cout << "speed improved" << endl;
    }
        if (pressed[WORLD_ATTACK_EVENT])
            attack(man_idx, 0);
}
