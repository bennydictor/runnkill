#include <game/init_world.h>
#include <game/world.h>
#include <iostream>
#include <game/man.h>
#include <game/field.h>
#include <cstdio>
#include <fstream>
#include <game/skills/skill_type.h>
#include <game/skills/bullet.h>
#include <game/init_world.h>
#include <math/geom.h>
#include <util/logstream.h>
#include <vector>
#include <map>
#include <cmath>
#include <ctime>
#include <cassert>


using namespace std;

map<string, int> man_idx_by_name;

vec3<float> get_rand_coords() {
    int i = rand() % world_w;
    int j = rand() % world_h;
    i /= 2;
    j /= 2;
    while (world_field[i][j] != 0) {
        ++j;
        if (j == world_h) {
            ++i;
            j = 0;
        }
    }
    return vec3<float>(float(i) + 0.5, 19, float(j) + 0.5); 
}

void in_skills() {
    fstream in;
    in.open("skills");
    string garbage;
    int amount, level;
    char type;
    default_skills.resize(3);
    skills_amounts.resize(3);
    for (int i = 0; i < 3; i++) {
        in >> garbage;
        in >> amount;
        default_skills[i].resize(amount);
        vector<int> levs;
        for (int j = 0; j < amount; j++) {
            in >> level >> type;
            cout << '!' << type << endl;
            if (type == 'M') 
                default_skills[i][j] = new melee_skill_t();
            else if (type == 'R')
                default_skills[i][j] = new range_skill_t();
            else if (type == 'T') 
                default_skills[i][j] = new trap_skill_t();
            else if (type == 'A') 
                default_skills[i][j] = new aura_skill_t();
            default_skills[i][j]->in_damage(in);
            levs.push_back(level);
        }
        skills_amounts[i].resize(100);
        int k = 0;
        for (int j = 0; j < 100; j++) {
            while (k < int(levs.size()) and levs[k] == j) {
                skills_amounts[i][j]++;
                k++;
            }
        }
        for (int j = 1; j < 100; j++) {
            skills_amounts[i][j] += skills_amounts[i][j - 1];
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
/*
void in_armours() {
    fstream in;
    in.open("armours");
    int am;
    in >> am;
    default_armours.resize(am);
    for (int j = 0; j < am; j++) {
        default_armours[j].in(in);
    }
    in.close();
}
*/
void in_animations() {
    fstream in;
    in.open("animations");
    int am;
    in >> am;
    animations.resize(am);
    for (int j = 0; j < am; j++) {
        animations[j].in(in);
    }
    in.close();
}
void in_materials() {
    fstream in;
    in.open("materials");
    int am;
    in >> am;
    float r, g, b, alpha;
    float *amb, *dif, *spec, shininess;
    fake_materials_idx.resize(am);
    for (int j = 0; j < am; j++) {
        in >> r >> g >> b;
        cout << r << ' '<< g<< ' ' << b << ' ' << endl;
        amb = cpp_make_vec3(r, g, b);
        in >> r >> g >> b;
        dif = cpp_make_vec3(r, g, b);
        in >> r >> g >> b;
        spec = cpp_make_vec3(r, g, b);
        in >> shininess >> alpha;
        fake_materials_idx[j] = make_material(amb, dif, spec, shininess, alpha).id;

    }
    in.close();
}

int init_world(void) {
    world_w = world_h = 40;
    cout << loglevel(LOG_I) << "world height: " << world_h << endl << "world width: " << world_w << endl << loglevel(LOG_D);
    world_field = gen_field_empty(world_w, world_h);
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
    in_animations();
    init_material();
    in_materials();
    cout << "Initialize of world succeed" << endl;
    return 0;
}

void add_player(char *name, int clazz) {
    printf("NEW PLAYER> %s\n", name);
    if (clazz == 3) {
        load_player(name);
        return;
    }
    persons.push_back(new man(name, clazz));
    string cppname(name);
    man_idx_by_name[cppname] = persons.back()->number;
    is_alive.push_back(2);
    persons.back()->coords = get_rand_coords();
    persons.back()->speed = vec3<float>(0, 0, 0);
}

void add_player(man* player) {
    persons.push_back(player);
    is_alive.push_back(2);
    persons.back()->coords = get_rand_coords();
    persons.back()->speed = vec3<float>(0, 0, 0);
}


void free_world(void) {
}
