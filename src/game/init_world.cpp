#include <game/init_world.h>
#include <game/world.h>
#include <iostream>
#include <game/man.h>
#include <game/field.h>
#include <cstdio>
#include <fstream>
#include <game/skill_type.h>
#include <game/bullet.h>
#include <game/init_world.h>
#include <math/geom.h>
#include <util/logstream.h>
#include <vector>
#include <cmath>
#include <ctime>
#include <cassert>


using namespace std;

void in_skills() {
    fstream in;
    in.open("skills");
    string garbage;
    int amount;
    char type;
    default_skills.resize(3);
    for (int i = 0; i < 3; i++) {
        in >> garbage;
        in >> amount;
        default_skills[i].resize(amount);
        for (int j = 0; j < amount; j++) {
            in >> type;
            cout << '!' << type << endl;
            default_skills[i][j].is_range = (type == 'R');
            default_skills[i][j].in_damage(in);
        }
    }
    in.close();
}

void in_items() {
    fstream in;
    in.open("items");
    int am;
    in >> am;
    default_items.resize(am);
    for (int j = 0; j < am; j++) {
        default_items[j].in(in);
    }
    in.close();
}

int init_world(void) {
    world_w = world_h = 100;
    cout << loglevel(LOG_I) << "world height: " << world_h << endl << "world width: " << world_w << endl << loglevel(LOG_D);
    world_field = gen_field_sun(world_w, world_h);
    gl_light_enable[0] = 1;
    gl_light[0].pos = make_vec3(100, 100, 100);
    gl_light[0].rot = make_vec3(3 * M_PI / 2, 0, 0);
    gl_light[0].fov = 1;
    gl_light[0].z_near = .1;
    gl_light[0].z_far = 1000;
    gl_light[0].ambient = make_vec3(.3, .3, .3);
    gl_light[0].diffuse = make_vec3(.4, .4, .4);
    gl_light[0].specular = make_vec3(1, 1, 1);
    in_skills();
    in_items();
    init_material();

    return 0;
}

void add_player(char *name, int clazz) {
    persons.push_back(new man(string(name), clazz));
    is_alive.push_back(true);
    persons.back()->coords = vec3<float>(0, 2, 0);
}

void free_world(void) {
}
