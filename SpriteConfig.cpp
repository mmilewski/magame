#include "StdAfx.h"

#include "SpriteConfig.h"
#include "Types.h"


SpriteConfig::SpriteConfig() {
    Insert("PlatformTopLeft"      , SpriteConfigData(DL::Foreground, 1, 1,   0*32, 1*32, 32, 32, false, false));
    Insert("PlatformLeft"         , SpriteConfigData(DL::Foreground, 1, 1,   1*32, 1*32, 32, 32, false, false));
    Insert("PlatformMid"          , SpriteConfigData(DL::Foreground, 1, 1,   0*32, 2*32, 32, 32, false, false));
    Insert("PlatformTop"          , SpriteConfigData(DL::Foreground, 1, 1,   1*32, 2*32, 32, 32, false, false));
    Insert("PlatformLeftTopRight" , SpriteConfigData(DL::Foreground, 1, 1,   2*32, 2*32, 32, 32, false, false));
    Insert("PlatformLeftRight"    , SpriteConfigData(DL::Foreground, 1, 1,   3*32, 2*32, 32, 32, false, false));
    Insert("PlatformTopRight"     , SpriteConfigData(DL::Foreground, 1, 1,   0*32, 3*32, 32, 32, false, false));
    Insert("PlatformRight"        , SpriteConfigData(DL::Foreground, 1, 1,   1*32, 3*32, 32, 32, false, false));
    Insert("EndOfLevel"           , SpriteConfigData(DL::Foreground, 4, 0.1, 4*32, 2*32, 32, 32, true, false));

    Insert("NcPlatformTopLeft"      , SpriteConfigData(DL::Background, 1, 1,   0*32, 1*32, 32, 32, false, true));
    Insert("NcPlatformLeft"         , SpriteConfigData(DL::Background, 1, 1,   1*32, 1*32, 32, 32, false, true));
    Insert("NcPlatformMid"          , SpriteConfigData(DL::Background, 1, 1,   0*32, 2*32, 32, 32, false, true));
    Insert("NcPlatformTop"          , SpriteConfigData(DL::Background, 1, 1,   1*32, 2*32, 32, 32, false, true));
    Insert("NcPlatformLeftTopRight" , SpriteConfigData(DL::Background, 1, 1,   2*32, 2*32, 32, 32, false, true));
    Insert("NcPlatformLeftRight"    , SpriteConfigData(DL::Background, 1, 1,   3*32, 2*32, 32, 32, false, true));
    Insert("NcPlatformTopRight"     , SpriteConfigData(DL::Background, 1, 1,   0*32, 3*32, 32, 32, false, true));
    Insert("NcPlatformRight"        , SpriteConfigData(DL::Background, 1, 1,   1*32, 3*32, 32, 32, false, true));

    Insert("mush_right", SpriteConfigData(DL::Entity, 4, 0.3,  0, 12 * 32, 32, 32, true, false));
    Insert("mush_left",  SpriteConfigData(DL::Entity, 4, 0.3,  0, 13 * 32, 32, 32, true, false));
    Insert("mush_stop",  SpriteConfigData(DL::Entity, 4, 0.3,  0, 14 * 32, 32, 32, true, false));

    Insert("player_right",        SpriteConfigData(DL::Player, 4, 0.1,     0,  4 * 32, 32, 32, true, false));
    Insert("player_left",         SpriteConfigData(DL::Player, 4, 0.1,     0,  5 * 32, 32, 32, true, false));
    Insert("player_stop",         SpriteConfigData(DL::Player, 5, 0.1,     0,  6 * 32, 32, 32, true, false));
    Insert("player_bullet",       SpriteConfigData(DL::Entity, 1, 0.3,  6*32, 13 * 32, 32, 32, true, false));
    Insert("twinshot_upgrade",    SpriteConfigData(DL::Entity, 4, 0.1,  6*32, 15 * 32, 32, 32, true, false));
    Insert("higherjump_upgrade",  SpriteConfigData(DL::Entity, 1, 1,    5*32,  4 * 32, 32, 32, true, false));
    Insert("orb",                 SpriteConfigData(DL::Entity, 1, 1,    4*32, 19 * 32, 32, 32, true, false));
    
    Insert("savepoint_off",       SpriteConfigData(DL::Entity, 1, 1,    3*32, 17 * 32, 32, 32, true, false));
    Insert("savepoint_on",        SpriteConfigData(DL::Entity, 4, 0.1,  4*32, 17 * 32, 32, 32, true, false));

    Insert("gui_eraser",     SpriteConfigData(DL::Entity, 1, 1,  10*32, 12*32, 32, 32, true, false));
}



SpriteConfigData SpriteConfig::Get(const std::string& name) const {
    if (Contains(name)) {
        return m_data.find(name)->second;
    }
    std::cerr << "Config not found: " << name << std::endl;
    throw("Config not found: " + name);
}

bool SpriteConfig::Contains(const std::string& name) const {
    return (m_data.find(name) != m_data.end());
}

void SpriteConfig::Insert(const std::string& name, const SpriteConfigData& data) {
    m_data.insert(std::make_pair(name, data));
}

