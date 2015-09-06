#include <iostream>
#include <man.h>
#include <field.h>
#include <cstdio>
#include <fstream>
#include <skill_type.h>
#include <bullet.h>
#include <vector>
#include <cmath>

#define EPS 1e-4
#define EXPLOSION_RAD 1e-2
#define INF 10000000
using namespace std;

const int len = 1;

vector<bullet> bullets;
vector<man*> persons;
vector<bool> is_alive, alive_bullets;
vector<vec3<float> > explosions;
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
void ternary_search_closest(vec3<float>& low, vec3<float>& hig, vec3<float> centre)
{
    for (int i = 0; i < 50; i++)
    {
        vec3<float> m1, m2;
        m1 = ((float)2 * low + hig) / (float)3;
        m2 = (low + (float)2 * hig) / (float)3;
        if (dist(m1, centre) > dist(m2, centre))
        {
            low = m1;
        }
        else
        {
            hig = m2;
        }
    }
}

bool is_intersected(vec3<float> centre, float rad, vec3<float> begin, vec3<float> end, vec3<float>& res) {
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
    if (dist(centre, l) >= rad - EPS) {
        return false;
    }
    l = begin;
    for (int i = 0; i < 30; i++) {
        m = (l + r) / (float)2;
        if (dist(centre, m) < rad) {
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
                if (dist(explosions.back(), persons[j]->coords) < MAN_RAD + EXPLOSION_RAD) {
                    int sector = detect_sector(persons[j]->coords, explosions.back(), persons[j]->orientation);
                    is_alive[j] = !persons[j]->take_damage(
                                count_dmg(persons[j]->body_parts[sector], bullets[b_idx].damage));
                    //Here will be effects adding
                }
            }

}
void move_bullet(int b_idx, float time) {
    if (!alive_bullets[b_idx]) {
        return;
    }
    vec3<float>our_point = bullets[b_idx].in_time(time);
    vec3<float>intersection;
    for (int i = 0; i < (int)persons.size(); i++) {
        if (is_intersected(persons[i]->coords, MAN_RAD, bullets[b_idx].coords,
            our_point, intersection)) {
            cerr << "Strike #" << i << endl;
            explosions.push_back(intersection);
            damage_last_explosion(b_idx);
            alive_bullets[b_idx] = 0;
            return;
        }
    }
    vec3<float> l, r, m, min_point(-INF, -INF, -INF);
    l = bullets[b_idx].coords;
    r = our_point;
    vec3<float> cube_p_d((int)r.x, (int)r.z, 0), cube_p_u((int)r.x, (int)r.z, F[(int)r.x][(int)r.z]);
    for (int i = 0; i < 30; i++) {
        m = (l + r) / (float)2;
        if (vec3<float>(cube_p_d, m).crs(vec3<float>(cube_p_d, cube_p_u)).y * 
            vec3<float>(cube_p_d, cube_p_u).crs(vec3<float>(cube_p_d, min_point)).y < 0) {
            l = m;
        }
        else {
            r = m;
        }
    }
    l = bullets[b_idx].coords;
    for (int i = 0; i < 30; i++) {
        m = (l + r) / (float)2;
        if (F[(int)m.x][(int)m.z] >= m.y) {
            r = m;   
        } else {
            l = m;               
        }
    }

    if (F[(int)l.x][(int)l.z] >= l.y) {
        explosions.push_back(l);
        alive_bullets[b_idx] = 0;
        damage_last_explosion(b_idx);
        return;
    }
    bullets[b_idx].coords = our_point; 
}

//vec3<float> ternary_search_closest(vec3<float> 

void attack(int man_idx, int idx) {

    man* z = persons[man_idx];
    if ((int)z->skills.size() <= idx or z->skills[idx].cost.mp > z->mp) {
        cerr << "You missed!" << endl;
        return;
    }
    cerr << "Well, " << endl;
    skill_t curr = z->skills[idx];
     if (curr.is_range) {
        bullets.push_back(bullet(curr.sample));
        bullets.back().coords = z->coords + (float)MAN_RAD * z->orientation;
        bullets.back().speed = vec3<float>(z->coords, bullets.back().coords);
        bullets.back().speed.resize(sqrt(z->speed.sqlen()) + curr.sample.speed.x);
        bullets.back().damage *= count_attack(*z);
        alive_bullets.push_back(1);
        cerr << "You shoot" << endl;
    } else {
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

void move_man(int idx, float time) {
    vec3<float> our_point = persons[idx]->in_time(time);
    vec3<float> low, hig;
    low.x = (int)persons[idx]->coords.x, hig.x = (int)persons[idx]->coords.x + 1;
    low.z = (int)persons[idx]->coords.z, hig.z = (int)persons[idx]->coords.z + 1;
    low.y = 0, hig.y = F[(int)low.x][(int)low.z];
    ternary_search_closest(low, hig, persons[idx]->coords);
    if (dist(low, persons[idx]->coords) < MAN_RAD)
    {
        move_man(idx, time / 2);
        if (time > EPS)
            move_man(idx, time / 2);
        persons[idx]->move(time);
        return;
    }
    for (int i = 0; i < (int)persons.size(); i++)
    {
        if (!is_alive[i] or i == idx)
            continue;
        if (dist(persons[i]->coords, persons[idx]->coords) < 2 * MAN_RAD)
        {
            move_man(idx, time / 2);
            if (time > EPS)
                move_man(idx, time / 2);
            persons[idx]->move(time);
            return;
        }
    }   
    persons[idx]->move(time);
    persons[idx]->coords = our_point;
}
/*
void what_to_draw(vector<obj> &result) {
    result.clear();
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            result.push_back(obj(ORTO));
            result.back().coord1 = vec3<float>(i, j, 0);
            result.back().coord2 = vec3<float>(i + 1, j + 1, F[i][j]);
        }
    }
    for (int i = 0; i < (int)persons.size(); i++) {
        result.push_back(obj(SPHERE));
        result.back().coord1 = persons[i].coords;
        result.height = 1;
    }
    for (int i = 0; i < (int)bullets.size(); i++) {
        result.push_back(obj(SPHERE));
        result.back().coord1 = bullets[i].coords'
        result.height = 0.1;
    }
}
*/
int main()
{
    ifstream skills;
    skills.open("skills");
    vector<vector<skill_t> > classes(3);
    string garbage;
    for (int i = 0; i < 3; i++) {
        skills >> garbage;
        int amount;
        char type;
        skills >> amount;
        for (int j = 0; j < amount; j++) {
            classes[i].push_back(skill_t());
            skills >> classes[i].back().cost.hp >> classes[i].back().cost.mp;
            skills >> type;
            classes[i].back().is_range = (type == 'R');
            classes[i].back().in_damage(skills);
        }
    }
    cerr << classes[0][0].is_range << ' ' << classes[0][0].u_l << endl;
    freopen("field", "w", stdout);
    w = h = 200;
    F = gen_field_empty(w, h);
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            cout << F[i][j];
        }
        cout << endl;
    }
    man z = man("z", WARRIOR);
    z.coords = vec3<float>(0, 1, 0);
    z.set_orientation(vec3<float>(3, 0.5, 0.5));
    persons.push_back(&z);
    is_alive.push_back(1);

    z.skills.push_back(classes[ARCHER][0]);
    man sample = man("sample", WARRIOR);
    persons.push_back(&sample);
    is_alive.push_back(1);

    sample.coords = vec3<float>(3, 1, 0);
    z.set_speed(vec3<float>(1, 0, 0));
    z.move(0.5);
    cerr << z.coords << endl;
    attack(0, 0);
    cerr << z.hp << ' ' << z.mp << ' ' << count_attack(z) << endl;
    cerr << "Look at it!" << endl;
    cerr << "----------" << endl;
    cerr << bullets[0].coords << ' ' << bullets[0].speed << alive_bullets[0] << endl;
    for (int i = 0; i < 10; i++) {
        move_bullet(0, 0.05);
        cerr << "Bullet #0 in " << bullets[0].coords << endl; 
        if (!alive_bullets[0]) {
            cerr << explosions[0] << endl;
        }
    }
    cerr << sample.hp << ' ' << sample.mp << endl;
    
    sample.fortify(RIGHT_UP);
    attack(0, 0);
    for (int i = 0; i < 10; i++) {
        cerr << "Bullet #1 in " << bullets[1].coords << endl; 
        move_bullet(1, 0.05);
    }
    cerr << sample.hp << ' ' << sample.mp << endl;
    man warrior, archer, mage;
    warrior = man("warrior", WARRIOR);

    archer = man("archer", ARCHER);
    mage = man("mage", MAGE);
    warrior.out(cerr);
    cerr << "Warrior:" << "hp = " << warrior.hp << "; mp = " << warrior.mp << "; attack = " << count_attack(warrior) << endl;
    cerr << "Now make font less and run look_at_map.py" << endl;
}
