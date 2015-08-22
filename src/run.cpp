#include <iostream>
#include <man.h>

using namespace std;

int main()
{
    man a = man("z", 0);
    cout << a.hp << ' ' << a.mn << endl;
}
