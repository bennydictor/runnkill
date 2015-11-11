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
#include <graphics/objects/box.h>
#include <graphics/objects/field.h>
#include <graphics/objects/sphere.h>
#include <graphics/objects/sphere_sector.h>
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
    w = h = 204;
    chunk = 12;
    world_map = new draw_obj*[w / chunk];
    for (int i = 0; i < w / chunk; ++i) {
        world_map[i] = new draw_obj[h / chunk];
    }
    assert(w % chunk == 0);
    assert(h % chunk == 0);
    F = gen_field_suns(w, h);
    world_max_height = 0;
    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            world_max_height = max(world_max_height, F[i][j]);
        }
    }
    init_field_object(w, h, F);
    for (int i = 0; i < w; i += chunk) {
        for (int j = 0; j < h; j += chunk) {
            world_map[i / chunk][j / chunk] = make_draw_subfield(i, j, i + chunk, j + chunk, default_material);
    //        cout << "chunk: [" << i << "-" << i + chunk << ")["<< j << "-" << j + chunk << ")" << endl;
        }
    }
    in_skills();
    in_items();

    int i = 1;
    int j = 1;
    while (F[i][j] <= 0)
    {
        j++;
        if (j == h)
        {
            j = 0;
            i++;
        }
    }
    cout << "i j: " << i << " " << j << endl;
    persons.push_back(new man("Derrior", 1));
    is_alive.push_back(1);
    persons[0]->coords = vec3<float>((float)i - 0.5, MAN_RAD, (float)j - 0.5);
    persons[0]->set_speed(vec3<float>(0, 0, 0));

    persons[0]->skills.push_back(default_skills[1][0]);
    persons[0]->body_parts[0].put_on(new item_t(default_items[0]));

    persons.push_back(new man("Benny", 1));
    is_alive.push_back(1);
    persons[1]->coords = vec3<float>((float)i + 0.5, MAN_RAD,(float)j + 0.5);
    persons[1]->set_speed(vec3<float>(0, 0, 0));
    persons[1]->skills.push_back(default_skills[1][0]);
    persons[1]->body_parts[0].put_on(new item_t(default_items[0]));
    persons[1]->fortify(LEFT_FRONT_UP);
    cout << persons[0]->skills.size() << endl;
    cout << persons[0]->skills[0].is_range << endl;
    cout << (default_skills[1][0].is_range) << endl;
    return 0;
}

void free_world(void) {
    for (int i = 0; i < w / chunk; ++i) {
        delete[] world_map[i];
    }
    delete[] world_map;
    free_field_object();
}
