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
    res.x = cos(angle) * s.x - sin(angle) * s.y;
    res.y = sin(angle) * s.x + cos(angle) * s.y;
    return res;
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
void move_bullet(int b_idx, float time) {
    if (!alive_bullets[b_idx]) {
        return;
    }
    vec3<float>our_point = bullets[b_idx].in_time(time);
    vec3<float>intersection;
    for (int i = 0; i < (int)persons.size(); i++) {
        if (is_intersected(persons[i]->coords, MAN_RAD, bullets[b_idx].coords,
            our_point, intersection)) {
            cerr << "Strike #" << i <<  endl;
            explosions.push_back(intersection);

            /*
            or (int j = 0; j < (int)persons.size(); j++) {
                if (dist(intersection, persons[i]->coords) < MAN_RAD + EXPLOSION_RAD) {
                    
                }
            }
            */

            alive_bullets[b_idx] = 0;
            return;
        }
    }
    if (F[(int)our_point.x][(int)our_point.z] > our_point.y) {
        vec3<float> l, r, m;
        l = bullets[b_idx].coords;
        r = our_point;
        for (int i = 0; i < 30; i++) {
            m = (l + r) / (float)2;
            if (F[(int)m.x][(int)m.z] >= m.y) {
                r = m;   
            } else {
                l = m;               
            }
        }
        explosions.push_back(l);
        alive_bullets[b_idx] = 0;
        return;
    }
    bullets[b_idx].coords = our_point; 
}
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
                        damage = all_dmg(persons[i]->body_parts[0], persons[i]->body_parts[4],
                                            persons[i]->body_parts[2], persons[i]->body_parts[6], z->skills[idx], attack);
                    } else {
                        cerr << "It`s 1" << endl;
                        damage = all_dmg(persons[i]->body_parts[1], persons[i]->body_parts[5],
                                            persons[i]->body_parts[3], persons[i]->body_parts[7], z->skills[idx], attack);
                    }
                } else {
                    if (He.z > I.z) {
                        cerr << "It`s 2" << endl;
                        damage = all_dmg(persons[i]->body_parts[2], persons[i]->body_parts[6],
                                            persons[i]->body_parts[1], persons[i]->body_parts[5], z->skills[idx], attack);
                    } else {
                        cerr << "It`s 3" << endl;
                        damage = all_dmg(persons[i]->body_parts[3], persons[i]->body_parts[7],
                                            persons[i]->body_parts[0], persons[i]->body_parts[4], z->skills[idx], attack);
                    }
                }
                is_alive[i] = !persons[i]->take_damage(damage);
            }
        }
    }
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
    man z = man("z", 0);
    z.coords = vec3<float>(0, 1, 0);

    persons.push_back(&z);
    is_alive.push_back(0);

    z.skills.push_back(classes[1][0]);
    man sample = man("y", 0);
    man warrior, archer, mage;
    persons.push_back(&sample);
    is_alive.push_back(1);

    sample.coords = vec3<float>(3, 1, 0);
    z.set_speed(vec3<float>(1, 0, 0));
    z.move(0.5);
    cerr << z.coords << endl;
    attack(0, 0);
    cerr << sample.hp << ' ' << sample.mp << endl;
    cerr << z.hp << ' ' << z.mp << ' ' << count_attack(z) << endl;
    cerr << "Look at it!" << endl;
    cerr << "----------" << endl;
    cerr << bullets[0].coords << ' ' << bullets[0].speed << alive_bullets[0] << endl;
    for (int i = 0; i < 10; i++) {
        move_bullet(0, 0.1);
        cerr << bullets[0].coords << ' ' << bullets[0].speed << alive_bullets[0] << endl;
        if (!alive_bullets[0]) {
            cerr << explosions[0] << endl;
        }
    }
    /*
    sample.fortify(RIGHT_UP);
    attack(0, 0);
    cerr << sample.hp << ' ' << sample.mp << endl;
    warrior = man("warrior", 0);
    archer = man("archer", 1);
    mage = man("mage", 2);
    warrior.out(cerr);
    cerr << "Warrior:" << "hp = " << warrior.hp << "; mp = " << warrior.mp << "; attack = " << count_attack(warrior) << endl
    cerr << "Now make font less and run look_at_map.py" << endl;
    */
}
