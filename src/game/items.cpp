#include <game/items.h>


using namespace std;

bool item_t::is_wearable(string bp_name) {
    return item_bp[type] == bp_name;
}
