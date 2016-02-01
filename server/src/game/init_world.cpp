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
            default_skills[i][j].type = type;
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
    float r, g, b;
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
        in >> shininess;
        fake_materials_idx[j] = make_material(amb, dif, spec, shininess).id;

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
    in_animations();
    init_material();
    in_materials();
/*
=======
    w = h = 500;
    chunk = 10;
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
    in_armours();
    in_animations();
    in_materials();
    int i = 0;
    int j = 0;
    while (F[i][j] != 0)
    {
        j++;
        if (j == h)
        {
            j = 0;
            i++;
        }
    }
    persons.push_back(new man("Derrior", 1));
    is_alive.push_back(1);
    persons[0]->coords = vec3<float>((float)i + 0.5, MAN_RAD, (float)j + 0.5);
    persons[0]->set_speed(vec3<float>(0, 0, 0));
    persons[0]->skills.push_back(default_skills[1][1]);
    cout << persons[0]->skills[0].type << endl;
    persons[0]->put_on(new item_t(default_items[0]), LEFT_BACK_DOWN);
    persons[0]->put_on(new item_t(default_items[2]), LEFT_BACK_DOWN);
    persons[0]->weapon = &default_armours[0];
    persons.push_back(new man("Benny", 1));
    is_alive.push_back(1);
    i = rand() % w;
    j = rand() % h;
    while (i < w and F[i][j] != 0)
    {
        j++;
        if (j == h)
        {
            j = 0;
            i++;
        }
    }
    i -= i / w;
    cout << "i j: " << i << " " << j << endl;

    persons[1]->coords = vec3<float>((float)i + 0.5, MAN_RAD,(float)j + 0.5);
    persons[1]->set_speed(vec3<float>(0, 0, 0));
    persons[1]->skills.push_back(default_skills[1][1]);
    persons[1]->fortify(LEFT_FRONT_UP);
    persons[1]->fortify(RIGHT_FRONT_UP);
    persons[1]->fortify(LEFT_FRONT_DOWN);
    persons[1]->fortify(RIGHT_FRONT_DOWN);
    cout << persons[0]->skills.size() << endl;
>>>>>>> origin/derrior
*/
    return 0;
}

void add_player(char *name, int clazz) {
    persons.push_back(new man(string(name), clazz));
    is_alive.push_back(2);
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
    persons.back()->coords = vec3<float>(i, 19, j);
    persons.back()->speed = vec3<float>(0, 0, 0);
}

void free_world(void) {
}
