#include <iostream>
#include <man.h>
#include <field.h>
#include <cstdio>
#include <fstream>
#include <skill_type.h>

using namespace std;

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
}
