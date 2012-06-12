#include "StdAfx.h"

#include "Engine.h"
#include "MushEntity.h"
#include "PlayerBulletEntity.h"
#include "TwinShotUpgrade.h"
#include "Misc.h"                // Orb
#include "EntityFactory.h"
#include "SavePoint.h"


EntityFactory::EntityFactory() {
}

SpritePtr GetSpriteByName(const std::string& name) {
    return SpritePtr(new Sprite(Engine::Get().GetSpriteConfig()->Get(name)));
}

EntityPtr EntityFactory::CreateEntity(ET::EntityType type,
                                      double x, double y,
                                      Vector2 direction) {
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
        SpritePtr sprite = GetSpriteByName("twinshot_upgrade");
        ptr->SetSprites(sprite, sprite, sprite);
    } else if (type == ET::HigherJump) {
        ptr.reset(new HigherJumpUpgrade(x, y));
        SpritePtr sprite = GetSpriteByName("higherjump_upgrade");
        ptr->SetSprites(sprite, sprite, sprite);
    } else if (type == ET::Orb) {
        ptr.reset(new Orb(x, y));
        SpritePtr sprite = GetSpriteByName("orb");
        ptr->SetSprites(sprite, sprite, sprite);
    } else if (type == ET::SavePoint) {
        SavePointPtr savepoint(new SavePoint(x,y));
        SpritePtr sprite_on = GetSpriteByName("savepoint_on");
        SpritePtr sprite_off = GetSpriteByName("savepoint_off");
        savepoint->SetSprites(sprite_on, sprite_off);
        ptr = savepoint;
    } else if (type == ET::Thorns) {
        ptr.reset(new Thorns(x, y));
        SpritePtr sprite = GetSpriteByName("thorns");
        ptr->SetSprites(sprite, sprite, sprite);
    } else if (type == ET::Arrow) {
        ptr.reset(new Arrow(x, y));
        SpritePtr left = GetSpriteByName("arrow_left"),
                  right = GetSpriteByName("arrow_right");
        ptr->SetSprites(left, right, SpritePtr());
    } else if (type == ET::Column) {
        ptr.reset(new Column(x, y));
        SpritePtr sprite = GetSpriteByName("column");
        ptr->SetSprites(sprite, sprite, sprite);
    } else if (type == ET::ArrowTrigger) {
        ptr.reset(new ArrowTrigger(x, y, direction.X()<0 ? ArrowTrigger::Left : ArrowTrigger::Right));
        SpritePtr right = GetSpriteByName("arrow_trigger_right"),
                  left = GetSpriteByName("arrow_trigger_left");
        ptr->SetSprites(left, right, SpritePtr());
    }

    if (!ptr) {
        std::cerr << "fabryka nie umie stworzyć żądanej jednostki na podstawie typu: " << type << ", " << x << ", " << y << std::endl;
    }
    return ptr;
}

EntityPtr EntityFactory::CreateEntity(const LevelEntityData& entity_data) {
    ET::EntityType et = StringAsEntityType(entity_data.name);
    if (et == ET::UNKNOWN) {
        std::cerr << "fabryka nie umie stworzyć żądanej jednostki na podstawie nazwy: "
                  << entity_data.name << std::endl;
        return EntityPtr();
    }
    return CreateEntity(et, entity_data.x, entity_data.y, entity_data.direction);
}
