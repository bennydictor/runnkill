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
vector<man> persons;
vector<bool> is_alive;
template <class T>

vec3<T> get_turned(vec3<T> s) {
    vec3<T> res(s);
    res.x = s.x - s.y;
    res.y = s.x + s.y;
    return res;
}

int all_dmg(body_part u_l_bp, body_part u_r_bp, body_part d_l_bp, body_part d_r_bp, skill_t skill) {
    return count_dmg(u_l_bp, skill.u_l) + count_dmg(u_r_bp, skill.u_r) + count_dmg(d_l_bp, skill.d_l) +
           count_dmg(d_r_bp, skill.d_r);

}
void attack(int man_idx, int idx) {

    man z = persons[man_idx];
    if ((int)z.skills.size() <= idx or z.skills[idx].cost.mn > z.mn) {
        return;
    }
    skill_t curr = z.skills[idx];
    if (curr.is_range) {
        bullets.push_back(bullet(curr.sample));
        bullets.back().coords = z.coords;
        bullets.back().speed.resize(sqrt(z.speed.sqlen()) + sqrt(curr.sample.speed.sqlen()));
    } else {
        for (size_t i = 0; i < persons.size(); i++) {
            if (i != man_idx and dist(persons[i].coords, z.coords) < len) {
                vec3<float> I, He;
                He.x = persons[i].coords.x;
                He.y = persons[i].coords.y;
                I.x = z.coords.x - He.x;
                I.y = z.coords.y - He.y;
                I = get_turned(I);
                int damage;
                if (He.x > I.x) {
                    if (He.y > I.y) {
                        damage = all_dmg(persons[i].body_parts[0], persons[i].body_parts[4],
                                            persons[i].body_parts[2], persons[i].body_parts[6], z.skills[idx]);
                    } else {
                        damage = all_dmg(persons[i].body_parts[1], persons[i].body_parts[5],
                                            persons[i].body_parts[3], persons[i].body_parts[7], z.skills[idx]);
                    }
                } else {
                    if (He.y > I.y) {
                        damage = all_dmg(persons[i].body_parts[2], persons[i].body_parts[6],
                                            persons[i].body_parts[1], persons[i].body_parts[5], z.skills[idx]);
                    } else {
                        damage = all_dmg(persons[i].body_parts[3], persons[i].body_parts[7],
                                            persons[i].body_parts[0], persons[i].body_parts[4], z.skills[idx]);
                    }
                }
                is_alive[i] = !persons[i].take_damage(damage);
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
    man a = man("z", 0);
    cerr << a.hp << ' ' << a.mn << endl;
    cerr << "Now make font less and run look_at_map.py" << endl;
    int x, y;
    for (int i = 0; i < 3; i++) {
        cin >> x >> y;
        vec3<int> a(x, y, 0), b;
        b = get_turned(a);
        cerr << b.x << ' ' << b.y << endl;
    }
}
