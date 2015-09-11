#ifndef ORTOHEDRON_H
#define ORTOHEDRON_H


struct ortohedron {
    vec3<float> p1, p2, p3, p4;
    ortohedron(vec3<float> a, vec3<float> b, vec3<float> c, vec3<float> d) {
        p1 = a;
        p2 = b;
        p3 = c;
        p4 = d;
    }
};

#endif // ORTOHEDRON_H
