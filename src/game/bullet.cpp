#include <game/bullet.h>
#include <iostream>
using namespace std;

bullet::bullet() {
    speed.x = speed.y = speed.z = 0;
}
vec3<float> bullet::in_time(float time) {
    return coords + time * speed;
}
void bullet::in(istream& stream) {
    stream >> speed.x >> rad;
}
