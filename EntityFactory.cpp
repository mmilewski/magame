#include "StdAfx.h"

#include "Engine.h"
#include "MushEntity.h"
#include "PlayerBulletEntity.h"
#include "TwinShotUpgrade.h"
#include "Misc.h"                // Orb
#include "EntityFactory.h"


EntityFactory::EntityFactory() {
}

SpritePtr GetSpriteByName(const std::string& name) {
    return SpritePtr(new Sprite(Engine::Get().GetSpriteConfig()->Get(name)));
}

EntityPtr EntityFactory::CreateEntity(ET::EntityType type, double x, double y) {
    const double eps = 0.0001;
    x += eps;  y += eps;

    EntityPtr ptr;
    if(type==ET::Mush) {
        ptr.reset(new MushEntity(x, y));
        ptr->SetSprites(GetSpriteByName("mush_left"), GetSpriteByName("mush_right"), GetSpriteByName("mush_stop") );
    } else if (type == ET::PlayerBullet) {
        ptr.reset(new PlayerBulletEntity(x, y));
        SpritePtr bullet = GetSpriteByName("player_bullet");
        ptr->SetSprites(bullet, bullet, bullet);
    } else if (type == ET::TwinShot) {
        ptr.reset(new TwinShotUpgrade(x, y));
        SpritePtr bullet = GetSpriteByName("twinshot_upgrade");
        ptr->SetSprites(bullet, bullet, bullet);
    } else if (type == ET::Orb) {
        ptr.reset(new Orb(x, y));
        SpritePtr bullet = GetSpriteByName("orb");
        ptr->SetSprites(bullet, bullet, bullet);
    }

    if (!ptr) {
        std::cerr << "fabryka nie umie stworzyć żądanej jednostki: " << type << ", " << x << ", " << y << std::endl;
    }
    return ptr;
}


EntityPtr EntityFactory::CreateEntity(const std::string& name, double x, double y) {
    if (name == "mush") {
        return CreateEntity(ET::Mush, x, y);
    } else if (name == "player_bullet") {
        return CreateEntity(ET::PlayerBullet, x, y);
    } else if (name=="twinshot_upgrade") {
        return CreateEntity(ET::TwinShot, x, y);
    } else if (name=="orb") {
        return CreateEntity(ET::Orb, x, y);
    }

    std::cerr << "fabryka nie umie stworzyć żądanej jednostki: " << name << std::endl;
    return EntityPtr();
}

EntityPtr EntityFactory::CreateEntity(const LevelEntityData& entity_data) {
    return CreateEntity(entity_data.name, entity_data.x, entity_data.y);
}
