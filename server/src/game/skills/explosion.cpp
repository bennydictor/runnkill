#include <game/skills/explosion.h>
#include <math/vec3.h>

using namespace std;

explosion::explosion() {
    coords = vec3<float>(0, 0, 0);
    end_time = EXPLOSION_TIME;
    time = 0;
    rad = EXPLOSION_RADIUS;
    damage = 0;
}

explosion::explosion(vec3<float> centre, float end, float r, float dmg) {
    coords = centre;
    end_time = end;
    time = 0;
    rad = r;
    damage = dmg;
}

float explosion::time_c() {
    return time / end_time;
}

