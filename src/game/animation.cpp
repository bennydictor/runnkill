#include <cmath>
#include <math/vec3.h>
#include <game/animation.h>
#include <game/util/event.h>
#include <util/smth.h>

using namespace std;

void animation::in(istream& stream) {
    int amount;
    stream >> amount;
    
    events.resize(amount);
    for (int i = 0; i < amount; i++) {
        events[i].in(stream);
    }
}

event animation::get(float time) {
    int l = 0, r = events.size();
    while (r - l > 1) {
        int m = (l + r) >> 1;
        if (events[m].dt < time)
            r = m;
        else
            l = m;
    }
    if (r == (int)events.size())
        return events.back();
    //swap(events[r], events[l]);
    float proportion = (time - events[l].dt) / (events[r].dt - events[l].dt);
    float angle = get_mid(events[l].angle, events[r].angle, proportion);
    vec3<float> p1 = get_mid(events[l].point1, events[r].point1, proportion);
    vec3<float> vec_to_p2 = vec3<float>(p1, get_mid(events[l].point2, events[r].point2, proportion));
    float fdist = events[l].get_dist();
    vec_to_p2.resize(get_mid(fdist, events[r].get_dist(), proportion));
    cout << p1 << vec_to_p2 << endl;
    event ret;
    ret.point1 = p1;
    ret.point2 = p1 + vec_to_p2;
    ret.angle = angle;
    //swap(events[r], events[l]);
    return ret;
}
