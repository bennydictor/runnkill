#include <iostream>
#include <man.h>
#include <field.h>
#include <cstdio>
#include <fstream>
#include <skill_type.h>
#include <bullet.h>
#include <vector>
#include <cmath>

using namespace std;

const int len = 1;

vector<bullet> bullets;
vector<man*> persons;
vector<bool> is_alive;
template <class T>

vec3<T> get_turned(vec3<T> s, float angle) {
    vec3<T> res(s);
    res.x = cos(angle) * s.x - sin(angle) * s.y;
    res.y = sin(angle) * s.x + cos(angle) * s.y;
    return res;
}

int all_dmg(body_part u_l_bp, body_part u_r_bp, body_part d_l_bp, body_part d_r_bp, skill_t skill, int attack) {
    cerr << "We want to count damage!" << endl;
    return count_dmg(u_l_bp, skill.u_l * attack) + count_dmg(u_r_bp, skill.u_r * attack) + count_dmg(d_l_bp, skill.d_l * attack) +
           count_dmg(d_r_bp, skill.d_r * attack);

}
void attack(int man_idx, int idx) {

    man* z = persons[man_idx];
    if ((int)z->skills.size() <= idx or z->skills[idx].cost.mn > z->mn) {
        cerr << "You missed!" << endl;
        return;
    }
    cerr << "Well, " << endl;
    skill_t curr = z->skills[idx];
    if (curr.is_range) {
        bullets.push_back(bullet(curr.sample));
        bullets.back().coords = z->coords;
        bullets.back().speed.resize(sqrt(z->speed.sqlen()) + sqrt(curr.sample.speed.sqlen()));
        bullets.back().damage *= count_attack(*z);
        cerr << "You shoot" << endl;
    } else {
        cerr << "you try to beat" << endl;
        for (size_t i = 0; i < persons.size(); i++) {
            if ((int)i != man_idx and dist(persons[i]->coords, z->coords) < len and z->speed.dot(vec3<float>(z->coords, persons[i]->coords))) {
                cerr << "You want to attack man #" << i << endl;
                vec3<float> I, He;
                He.x = persons[i]->coords.x;
                He.y = persons[i]->coords.y;
                I.x = z->coords.x - He.x;
                I.y = z->coords.y - He.y;
                I = get_turned(I, M_PI / 4 + atan2(persons[i]->coords.y, persons[i]->coords.x));
                cerr << "Yeah, we can detected an attack direction!" << endl;
                int damage;
                int attack = count_attack(*z);
                if (He.x > I.x) {
                    if (He.y > I.y) {
                        cerr << "It`s 0" << endl;
                        damage = all_dmg(persons[i]->body_parts[0], persons[i]->body_parts[4],
                                            persons[i]->body_parts[2], persons[i]->body_parts[6], z->skills[idx], attack);
                    } else {
                        cerr << "It`s 1" << endl;
                        damage = all_dmg(persons[i]->body_parts[1], persons[i]->body_parts[5],
                                            persons[i]->body_parts[3], persons[i]->body_parts[7], z->skills[idx], attack);
                    }
                } else {
                    if (He.y > I.y) {
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
            skills >> classes[i].back().cost.hp >> classes[i].back().cost.mn;
            skills >> type;
            classes[i].back().is_range = (type == 'R');
            classes[i].back().in_damage(skills);
        }
    }
    cerr << classes[0][0].is_range << ' ' << classes[0][0].u_l << endl;
    freopen("field", "w", stdout);
    int w = 200, h = 200;
    int** F = gen_field_sun(w, h);
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            cout << F[i][j];
        }
        cout << endl;
    }
    man z = man("z", 0);
    z.coords = vec3<float>(0, 0, 0);

    persons.push_back(&z);
    is_alive.push_back(1);

    z.skills.push_back(classes[0][0]);
    man sample = man("y", 0);
    man warrior, archer, mage;
    persons.push_back(&sample);
    is_alive.push_back(1);

    sample.coords = vec3<float>(1, 0, 0);
    z.set_speed(vec3<float>(1, 0, 0));
    z.move(0.5);
    cerr << z.coords << endl;
    attack(0, 0);
    sample.fortify(RIGHT_UP);
    cerr << sample.hp << ' ' << sample.mn << endl;
    cerr << z.hp << ' ' << z.mn << ' ' << count_attack(z) << endl;
    
    attack(0, 0);
    cerr << sample.hp << ' ' << sample.mn << endl;
    warrior = man("warrior", 0);
    archer = man("archer", 1);
    mage = man("mage", 2);
    cerr << "Warrior:" << "hp = " << warrior.hp << "; mn = " << warrior.mn << "; attack = " << count_attack(warrior) << endl;
    cerr << "Now make font less and run look_at_map.py" << endl;
}
