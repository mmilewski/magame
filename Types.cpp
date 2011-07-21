#include "StdAfx.h"
#include "Types.h"

std::string EntityTypeAsString(ET::EntityType et) {
    switch (et) {
    case ET::PlayerBullet : return "player_bullet";
    case ET::SingleShot   : return "singleshot_upgrade";
    case ET::TwinShot     : return "twinshot_upgrade";
    case ET::Mush         : return "mush";
    case ET::Orb          : return "orb";

    case ET::UNKNOWN      : return "unknown";
    case ET::COUNT        : assert(false && "count nie jest typem jednostki"); break;
    }
    assert(false && "nieobsłużony typ jednostki");
}
