#include "StdAfx.h"

#include "SpriteConfig.h"
#include "Types.h"


SpriteConfig::SpriteConfig() {
    Insert("player_right", SpriteConfigData(DL::Player, 5, 0.2, 0, 4 * 32, 32, 32, true));
    Insert("player_left",  SpriteConfigData(DL::Player, 5, 0.2, 0, 5 * 32, 32, 32, true));
    Insert("player_stop",  SpriteConfigData(DL::Player, 1, 0.2, 0, 6 * 32, 32, 32, true));

    Insert("platform_left",  SpriteConfigData(DL::Foreground, 1, 1, 0, 1*32, 32, 32, true));
    Insert("platform_mid",   SpriteConfigData(DL::Foreground, 1, 1, 0, 2*32, 32, 32, true));
    Insert("platform_right", SpriteConfigData(DL::Foreground, 1, 1, 0, 3*32, 32, 32, true));
    Insert("end_of_level",   SpriteConfigData(DL::Foreground, 1, 1, 32, 2*32, 32, 32, true));

    Insert("mush_right", SpriteConfigData(DL::Entity, 2, 0.3,  0, 11 * 32, 32, 32, true));
    Insert("mush_left",  SpriteConfigData(DL::Entity, 2, 0.3,  0, 11 * 32, 32, 32, true));
    Insert("mush_stop",  SpriteConfigData(DL::Entity, 1, 0.3,  0, 12 * 32, 32, 32, true));

    Insert("player_bullet",  SpriteConfigData(DL::Entity, 2, 0.2,  3*32, 11*32, 32, 32, true));

    Insert("twinshot_upgrade", SpriteConfigData(DL::Entity, 2, 0.3,  0*32, 13*32, 32, 32, true));
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

