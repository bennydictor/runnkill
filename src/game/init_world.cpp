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
    world_w = world_h = 100;
    chunk = 10;
    cout << loglevel(LOG_I) << "world height: " << world_h << endl << "world width: " << world_w << endl << "chunk size: " << chunk << endl << loglevel(LOG_D);
    assert(world_w % chunk == 0);
    assert(world_h % chunk == 0);
    world_map = new draw_obj*[world_w / chunk];
    for (int i = 0; i < world_w / chunk; ++i) {
        world_map[i] = new draw_obj[world_h / chunk];
    }
    world_field = gen_field_sun(world_w, world_h);
    world_max_height = 0;
    for (int i = 0; i < world_w; ++i) {
        for (int j = 0; j < world_h; ++j) {
            world_max_height = max(world_max_height, world_field[i][j]);
        }
    }
    for (int i = 0; i < world_w; i += chunk) {
        for (int j = 0; j < world_h; j += chunk) {
            world_map[i / chunk][j / chunk] = make_draw_subfield(i, j, i + chunk, j + chunk, default_material);
        }
    }
    in_skills();
    in_items();

    return 0;
}

void free_world(void) {
    for (int i = 0; i < world_w / chunk; ++i) {
        delete[] world_map[i];
    }
    delete[] world_map;
}
