#include <iostream>
#include <man.h>
#include <field.h>
#include <cstdio>

using namespace std;

int main()
{
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
