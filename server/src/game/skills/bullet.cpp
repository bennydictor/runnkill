#include <game/skills/bullet.h>
#include <iostream>
using namespace std;

bullet::bullet() {
    speed.x = speed.y = speed.z = 0;
}
vec3<float> bullet::in_time(float time) {
    return coords + time * speed;
}
void bullet::in(istream& stream) {
    stream >> speed.x >> rad >> exp_rad >> bullet_m_idx >> explosion_m_idx; /*abs of speed, radius of bulet, 
                                                                              radius of explosion, material idx of bullet,
                                                                              material index of explosion */
}

void bullet::upgrade(float attack) {
    for (effect& k : effects) {
        k.upgrade(attack);
    }
}
