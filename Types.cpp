#include "StdAfx.h"
#include "Types.h"

std::map<ET::EntityType, std::string> et_to_str;
std::map<std::string, ET::EntityType> str_to_et;

void Insert(ET::EntityType et, std::string str) {
    et_to_str[et] = str;
    str_to_et[str] = et;
}

void EnsureEntityTypeToStringMapping() {
    if (!et_to_str.empty() || !str_to_et.empty()) {
        return;
    }

    // bonusy
    Insert(ET::SingleShot, "singleshot_upgrade");
    Insert(ET::TwinShot, "twinshot_upgrade");
    Insert(ET::HigherJump, "higherjump_upgrade");
    Insert(ET::Orb, "orb");

    // przeciwnicy
    Insert(ET::Mush, "mush");
    Insert(ET::Arrow, "arrow");

    // inne
    Insert(ET::PlayerBullet, "player_bullet");
    Insert(ET::SavePoint, "savepoint");
    Insert(ET::Thorns, "thorns");


// w pliku poziomu występuj `arrow_trigger_left` oraz `_right`, żeby określić
// w którą stronę ma być zwrócony wyzwalacz. Przez to musi on posiadać ifa w
// EntityFactory -- parser pliku poziomu obsługuje tylko pozycję encji.
//    Insert(ET::ArrowTrigger, "arrow_trigger");
}

std::string EntityTypeAsString(ET::EntityType et) {
    EnsureEntityTypeToStringMapping();

    if (et_to_str.find(et) == et_to_str.end()) {
        std::stringstream ss;
        ss << "Nie znaleziono ciągu znaków dla " << et;
        throw std::invalid_argument(ss.str());
    }
    return et_to_str.at(et);
}

ET::EntityType StringAsEntityType(std::string str) {
    EnsureEntityTypeToStringMapping();

    if (str_to_et.find(str) == str_to_et.end()) {
        std::stringstream ss;
        ss << "Nie znaleziono typu jednostki dla ciągu " << str;
        throw std::invalid_argument(ss.str());
    }
    return str_to_et.at(str);
}
