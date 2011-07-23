#include "StdAfx.h"
#include "Types.h"

std::string EntityTypeAsString(ET::EntityType et) {
    switch (et) {

    // bonusy
    case ET::SingleShot   : return "singleshot_upgrade";
    case ET::TwinShot     : return "twinshot_upgrade";
    case ET::HigherJump   : return "higherjump_upgrade";
    case ET::Orb          : return "orb";

    // przeciwnicy
    case ET::Mush         : return "mush";

    // inne
    case ET::PlayerBullet : return "player_bullet";
    case ET::SavePoint    : return "savepoint";

    case ET::UNKNOWN      : return "unknown";
    case ET::COUNT        : assert(false && "count nie jest typem jednostki"); break;
    }

    std::stringstream msg ("nieobsłużony typ jednostki: ");
    msg << et;
    throw new std::logic_error(msg.str());
}
