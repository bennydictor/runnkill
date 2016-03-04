#include <game/man.h>
#include <game/common.h>
#include <game/animation.h>
#include <cassert>
#include <cstdlib>

int GAME_MAX_MAN_IDX;

using namespace std;

void man::die() {
    exp = float(exp) * 0.9;
    for (effect& k : effects) {
        k.time = 0;
    }
    move(0);
    touch_ground = false;
    is_stunned = 0;
    is_running = false;
    speed = vec3<float>(0, 0, 0);
    hp = 0;
}

man::man() {
    cls = 0;
    def_mod = atk_mod = 1; 
    can_die = true;
    have_shield = false;
    is_stunned = 0;
    touch_ground = false;
    my_aura = NULL;
//    weapon = NULL;
    orientation = vec3<float>(1, 0, 0);
    for (size_t i = 0; i < BP_AMOUNT; i++) {
        body_parts.push_back(body_part(bp_names[i], bp_init_mods[i]));
    }
    init_values(hp, mp, agility, strength, intellect, abs_speed, jump_high, attack_rad, cls);
    max_hp = hp;
    max_mp = mp;
    curr_skill = -1;

    exp = level = 0;
    level_exp = exp_to_next_level[0];
    number = GAME_MAX_MAN_IDX++;
    recovery.hp = 0;
    recovery.mp = 3;
}
man::man(string _name, int cl) {
    name = _name;
    cls = cl;
    def_mod = atk_mod = 1; 
    can_die = true;
    have_shield = false;
    is_stunned = false;
    touch_ground = false;
    my_aura = NULL;
//    weapon = NULL;
    orientation = vec3<float>(1, 0, 0);
    for (size_t i = 0; i < BP_AMOUNT; i++) {
        body_parts.push_back(body_part(bp_names[i], bp_init_mods[i]));
    }
    init_values(hp, mp, agility, strength, intellect, abs_speed, jump_high, attack_rad, cl);
    busy = exp = level = 0;
    level_exp = exp_to_next_level[0];
    speed = vec3<float>(abs_speed, 0, 0);
    number = GAME_MAX_MAN_IDX++;
    max_hp = hp;
    max_mp = mp;
    curr_skill = -1;
    recovery.hp = 0;
    recovery.mp = 3;
}

man::man(istream& file) {
    file >> name >> cls >> def_mod >> atk_mod >> can_die >> have_shield;
    touch_ground = false;
    is_stunned = false;
    my_aura = NULL;
    orientation = vec3<float>(1, 0, 0);

    for (size_t i = 0; i < BP_AMOUNT; i++) {
        body_parts.push_back(body_part(bp_names[i], bp_init_mods[i]));
    }

    file >> hp >> mp >> agility >> strength >> intellect >> abs_speed >> jump_high >> attack_rad;
    file >> exp >> level >> level_exp;
    speed = vec3<float>(abs_speed, 0, 0);
    file >> number;
    max_hp = hp;
    max_mp = mp;
    curr_skill = -1;
    file >> recovery.hp >> recovery.mp;
    cout << "ok" << endl;
}

void man::set_speed(vec3<float> spd) {
    speed = spd;
}

void man::set_orientation(vec3<float> orient) {
    orientation = orient;
    orientation.resize(1);
}
void man::get_effect_1(mod_t res) {
    take_damage(res.hp, res.owner);   
    mp -= res.mp;
}

void man::get_effect_2(mod_t res) {
    if (res.agility > EPS_FOR_SKILLS) 
        agility *= res.agility;
    if (res.strength >  EPS_FOR_SKILLS) 
        strength *= res.strength;
    if (res.intellect > EPS_FOR_SKILLS) 
        intellect *= res.intellect;
    if (res.speed > EPS_FOR_SKILLS) 
        abs_speed *= res.speed;
    if (res.def_mod > EPS_FOR_SKILLS) 
        def_mod *= res.def_mod;
    if (res.atk_mod > EPS_FOR_SKILLS) 
        atk_mod *= res.atk_mod;
    //cout << '!' << abs_speed << endl; 
}

void man::add_effect(effect a, vec3<float> _coords) {
    //cout << a.mods_one_side.hp << endl;
    effects.push_back(a);
    get_effect_2(a.mods_two_side);
    if (a.is_stunning) {
        is_stunned++;
    }
    if (a.is_moving) {
        touch_ground = false;
        vec3<float> orient;
        if (a.type) {
            orient = vec3<float>(coords, _coords);
            if (dist(coords, _coords) < EPS_FOR_SKILLS) {
                orient = orientation;
            }
            orient.resize(1);
        } else {
            orient = (orientation);
        }
        float angle = atan2(orient.z, orient.x);
        a.speed.rotate(angle, 0, 0);
        speed = speed + a.speed;
        cout << speed << endl;
        a.is_moving = 0;
    }
}
vec3<float> man::in_time(float time) {
    if (!(time > 0)) {
        cout << "BAD TIME" << endl;
        return coords;
    }
    int amount_of_f = 0;
    for (int j = 0; j < BP_AMOUNT; j++) {
        if (body_parts[j].is_fortified) {
            amount_of_f++;
        }
    }
    if (have_shield) {
        amount_of_f--;
    }
    //cout << speed << endl;
    vec3<float> speed_tmp = speed;
    speed_tmp.y = 0;
    vec3<float> ret = coords + (float)((time * max(0.1f, (1 - (float)0.2 * amount_of_f))) * (is_running ? (have_shield ? 1.3 : 1.6) : 1)) * speed_tmp;
    ret.y += time * speed.y;
    char a;
    if (!(abs(speed.x) >= 0))
        cin >> a;
    //cout << ret << ' ' << speed << endl;
    return ret;
}

void man::move(float time) {
    if (time < 0)
        return;
    if (!(time >= 0 or time <= 0)) {
        cout << "WE RECIEVED NAN" << endl;
        assert(time >= 0 or time <= 0);
        return;
    }
    vector<effect> new_effects;
    for (size_t i = 0; i < effects.size(); i++) {
        mod_t res = effects[i].tic(time);
        get_effect_1(res);
        if (effects[i].time >= 0 and effects[i].time - time <= 0) {
            get_effect_2(effects[i].mods_two_side * -1);
            if (effects[i].is_stunning) {
                is_stunned--;
            }
        }
        effects[i].time -= time;
        if (effects[i].time > -EPS_FOR_SKILLS) {
            new_effects.push_back(effects[i]);
        }
    }
    effects = new_effects;
    for (int i = 0; i < (int)skills.size(); i++) {
        skills[i]->to_activate_skill -= time; 
    }
    for (int i = 0; i < (int)body_parts.size(); i++) {
        body_parts[i].can_changed = max(0.0, body_parts[i].can_changed - time);
    }
    //cout << busy << endl;
    if (!is_running) {
        busy -= time;
        if (busy < -EPS_FOR_SKILLS) {
            curr_skill = -1;
            busy = 0;
        }
    }
    hp += (recovery.hp * time / (1 + int(is_running)));
    hp = min(hp, (float)max_hp);
    mp += (recovery.mp * time / (1 + int(is_running)));
    mp = min(mp, (float)max_mp);
    mp = max(mp, 0.0f);
    vector<message> new_messages;
    for (int i = 0; i < (int)messages.size(); i++) {
        messages[i].time -= time;
        if (messages[i].time > 0) {
            new_messages.push_back(messages[i]);
        }
    }
    if (my_aura) {
        my_aura->time -= time;
        if (my_aura->can_use < 0) {
            my_aura->can_use = my_aura->tic;
        }
        my_aura->can_use -= time;
        if (my_aura->time < 0) {
            my_aura = NULL;
        }
    }
}

bool man::take_damage(float dmg, int p) {
    //cout << dmg << endl;
    if (can_die) {
        dmg = min(dmg, float(hp + EPS_FOR_SKILLS));
    } else {
        dmg = min(dmg, hp - 1);
    }
    dmg = max(dmg, hp - max_hp);
    if (p != number) {
        if (dmg < 0) {
            healers[p] -= dmg;
            vector<int> to_erase;
            for (auto it = damagers.begin(); it != damagers.end(); it++) {
                it->second += dmg * (it->second / sum_damage);
                if (it->second < 0) {
                    to_erase.push_back(it->first);
                }
            }
            for (int i : to_erase) {
                damagers.erase(i);
            }
            sum_damage += dmg;
        } else {
            damagers[p] += dmg;
            vector<int> to_erase;
            for (auto it = healers.begin(); it != healers.end(); it++) {
                it->second -= dmg * (it->second / sum_damage);
                if (it->second < 0) {
                    to_erase.push_back(it->first);
                }
            }
            for (int i : to_erase) {
                healers.erase(i);
            }
            sum_damage += dmg;
        }
    }
    hp -= dmg;
    return (hp < EPS_FOR_SKILLS);
    
}
int count_attack(man z) {
    return atk_strength_mods[z.cls] * z.strength + atk_agi_mods[z.cls] * z.agility + atk_int_mods[z.cls] * z.intellect;
}

void man::fortify(int idx) {
    if (body_parts[idx].can_changed > 0)
        return;
    body_parts[idx].is_fortified ^= 1;
    body_parts[idx].can_changed = TIME_AFTER_FORTIFY;
}

void man::out(ostream& stream) {
    stream << name << ' ' << (cls == 0 ? "Warrior" : ((cls == 1) ? "Archer" : "Mage")) << ", " << level << " lvl, " << endl;
    stream << hp << ' ' << mp << endl;
    stream << "My attack = " << count_attack(*this) << endl;
    stream << "I`m in " << coords << endl;
}

void man::run(bool must_run) {
    if (must_run) {
        if (!is_running) {
            is_running = true;
            curr_skill = -1;
            busy = 0;
            need_to_cast = 0;
            for (int i = 0; i < (int) body_parts.size(); i++)
            {
                body_parts[i].is_fortified = false;
            }
            if (have_shield)
            {
                body_parts[LEFT_FRONT_UP].is_fortified = true;
            }
        }
    } else {
        is_running = false;
    }
}

void man::put_on(item_t* item, int idx) {
    item_t* wore = body_parts[idx].put_on(item);
    if (wore)
    {
        get_effect_2(wore->effects * -1);
        max_hp -= wore->hp;
        max_mp -= wore->mp;
       
        bag.push_back(wore);
    }
    get_effect_2(item->effects);
    max_hp += item->hp;
    max_mp += item->mp;
}

void man::new_message(char* s, float t) {
    messages.push_back(message());
    messages.back().str = s;
    messages.back().time = t;
}

char* man::get_text() {
    char* curr = buff;
    for (int i = 0; i < int(messages.size()); i++) {
        memcpy(curr, messages[i].str, messages[i].len());
        curr += messages[i].len();
        *curr = '\n';
        curr++;
    }
    *curr = 0;
    return buff;
}

void man::get_exp(int e) {
    exp += e;
    char m[200];
    sprintf(m, "New points: %d!", e);
    new_message(m, 3);
    if (exp > level_exp) {
        level++;
        sprintf(m, "New level: %d! Congratulations!", level);
        new_message(m, 5);
        exp -= level_exp;
        level_exp = exp_to_next_level[level];
    }
}

void man::write_info(ostream& file) {
    vector<effect> my_effects = effects;
    for (effect& k : effects) {
        k.time = 0;
    }
    move(0);

    file << ' ' << name << ' ' << cls << ' ' << def_mod << ' ' << atk_mod << ' ' << can_die << ' ' << have_shield << endl;
    file << ' ' << max_hp << ' ' << max_mp << ' ' << agility << ' ' << strength << ' ' << intellect << ' ' << abs_speed << ' ' << jump_high << ' ' << attack_rad;
    file << ' ' << exp << ' ' << level << ' ' << level_exp;
    file << ' ' << number;
    file << ' ' << recovery.hp << ' ' << recovery.mp;
    effects.clear();
    for (effect& k : my_effects) {
        add_effect(k, coords);
    }
    move(0);
}

void man::respawn() {
    
    hp = max_hp;
    mp = max_mp;
}

bool man::can_cast(int idx) {
    return !(busy > 0 or (int)skills.size() <= idx 
    or skills[idx]->cost.mp > mp or 
    skills[idx]->to_activate_skill > EPS_FOR_SKILLS 
    or is_stunned or is_running);
}

